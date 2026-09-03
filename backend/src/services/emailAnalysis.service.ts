import Anthropic from "@anthropic-ai/sdk";
import { zodOutputFormat } from "@anthropic-ai/sdk/helpers/zod";
import { z } from "zod";
import { env } from "../config/env.js";
import { AppError } from "../lib/errors.js";
import { logger } from "../lib/logger.js";
import { toAppError } from "./claude.service.js";
import type { EmailMessage } from "./email/types.js";

/**
 * Vēstuļu analīze: atrod abonementu notikumus un krāpšanas mēģinājumus.
 *
 * PRIVĀTUMS. Atšķirībā no bankas darījumiem, šeit uz API AIZIET vēstules
 * saturs — bez tā klasificēt nav iespējams. Toties:
 *   - tiek lasītas tikai vēstules, kas atbilst šauram meklēšanas vaicājumam
 *     (skat. buildSearchQuery), nevis visa pastkastīte
 *   - ķermenis tiek apgriezts līdz 2000 rakstzīmēm
 *   - datubāzē tiek glabāti TIKAI izvilktie fakti, nevis vēstules teksts
 * Tas ir jāpasaka lietotājam skaidri, nevis jāslēpj.
 */

const SYSTEM_PROMPT = `Tu esi asistents, kas lasa cilvēka e-pastu, lai pasargātu viņu no
nevajadzīgiem maksājumiem un krāpšanas. Vēstules ir holandiešu vai angļu valodā.

Katrai vēstulei nosaki, vai tā ir viens no šiem:

- trial_ending    — bezmaksas izmēģinājums drīz beigsies un sāksies maksa
- price_change    — pakalpojuma sniedzējs paziņo par cenas maiņu
- renewal         — abonements tiks automātiski atjaunots (bieži gada)
- invoice         — parasts rēķins vai norēķina paziņojums par esošu pakalpojumu
- suspicious      — krāpšanas mēģinājums

Krāpšanas pazīmes, kas jāņem vērā:
- sūtītāja domēns neatbilst uzņēmumam, par kuru vēstule izliekas
- steidzināšana ar draudiem ("24 stundu laikā", "konts tiks bloķēts")
- prasība ievadīt paroli, PIN, pilnu kartes numuru vai CVV
- saite uz domēnu, kas atdarina īsto ar papildvārdiem vai defisēm
- "balva" vai "laimests", kur sīkā drukā slēpjas ikmēneša maksa

SVARĪGI: īsts rēķins no īsta pakalpojuma sniedzēja NAV krāpšana, pat ja summa
ir liela. Nepārcenties — kļūdaini atzīmēts īsts rēķins liek cilvēkam nokavēt
maksājumu.

Vēstules, kas nav neviens no minētajiem (reklāmas, jaunumi, personiska sarakste),
vienkārši izlaid — neiekļauj tās atbildē.

Summas atgriez kā pozitīvus skaitļus. Datumus formātā YYYY-MM-DD.
confidence ir no 0 līdz 1.`;

export const FINDING_KINDS = [
  "trial_ending",
  "price_change",
  "renewal",
  "invoice",
  "suspicious",
] as const;

const findingSchema = z.object({
  /** Sasaista ar ievadē doto vēstuli. */
  external_id: z.string().min(1),
  kind: z.enum(FINDING_KINDS),
  merchant_name: z.string().min(1),
  /** Īss skaidrojums latviski, ko lietotājs redzēs lietotnē. */
  summary: z.string().min(1),
  amount: z.number().nullable(),
  currency: z.string().nullable(),
  effective_date: z
    .string()
    .regex(/^\d{4}-\d{2}-\d{2}$/)
    .nullable(),
  confidence: z.number().min(0).max(1),
  /** Aizpildīts tikai kind === "suspicious": kāpēc tas izskatās pēc krāpšanas. */
  risk_reasons: z.array(z.string()),
});

const analysisSchema = z.object({ findings: z.array(findingSchema) });

export type EmailFindingResult = z.infer<typeof findingSchema>;

let client: Anthropic | null = null;

function getClient(): Anthropic {
  if (!env.ANTHROPIC_API_KEY) {
    throw AppError.unavailable(
      "ANTHROPIC_API_KEY nav konfigurēta — vēstuļu analīze nav pieejama.",
    );
  }
  client ??= new Anthropic({ apiKey: env.ANTHROPIC_API_KEY });
  return client;
}

/**
 * Sagatavo pieprasījumu. Atdalīts no nosūtīšanas, lai to varētu pārbaudīt ar
 * testiem — jo īpaši to, kas tieši no vēstules aiziet uz API.
 */
export function buildEmailAnalysisRequest(messages: EmailMessage[]) {
  // Skaidri uzskaitām laukus: adresāts, etiķetes un pārējie Gmail metadati
  // uz API neaiziet.
  const payload = messages.map((message) => ({
    external_id: message.externalId,
    received_at: message.receivedAt.toISOString().slice(0, 10),
    sender_name: message.senderName,
    sender_address: message.senderAddress,
    subject: message.subject,
    body: message.body,
  }));

  return {
    model: env.ANTHROPIC_MODEL,
    max_tokens: 16000,
    system: SYSTEM_PROMPT,
    thinking: { type: "adaptive" as const },
    messages: [
      {
        role: "user" as const,
        content: [
          "Šeit ir vēstules:",
          "",
          JSON.stringify(payload, null, 2),
          "",
          "Atgriez atradumus norādītajā formātā. Vēstules, kas neietilpst nevienā",
          "kategorijā, izlaid.",
        ].join("\n"),
      },
    ],
    output_config: { format: zodOutputFormat(analysisSchema) },
  };
}

export async function analyzeEmails(
  messages: EmailMessage[],
): Promise<EmailFindingResult[]> {
  if (messages.length === 0) return [];

  const anthropic = getClient();

  try {
    const message = await anthropic.messages.parse(
      buildEmailAnalysisRequest(messages),
    );

    if (!message.parsed_output) {
      throw AppError.upstream("AI atbilde neatbilda gaidītajam formātam");
    }

    const findings = message.parsed_output.findings;

    logger.info("Vēstuļu analīze pabeigta", {
      emails: messages.length,
      findings: findings.length,
      suspicious: findings.filter((f) => f.kind === "suspicious").length,
      inputTokens: message.usage.input_tokens,
      outputTokens: message.usage.output_tokens,
    });

    return findings;
  } catch (error) {
    throw toAppError(error, "Vēstuļu analīze");
  }
}
