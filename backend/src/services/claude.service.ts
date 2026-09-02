import Anthropic from "@anthropic-ai/sdk";
import { zodOutputFormat } from "@anthropic-ai/sdk/helpers/zod";
import { z } from "zod";
import { env } from "../config/env.js";
import { AppError } from "../lib/errors.js";
import { logger } from "../lib/logger.js";

/**
 * Claude API integrācija: darījumu kategorizēšana un atcelšanas melnraksti.
 *
 * Privātums: uz API tiek sūtīti TIKAI datums, apraksts un summa. Lietotāja
 * e-pasts, konta numurs, ID un jebkuri citi personas dati nekad neizceļo no
 * mūsu datubāzes.
 */

const SYSTEM_PROMPT = `Tu esi finanšu asistents, kas analizē bankas darījumu vēsturi, lai atrastu
atkārtotus maksājumus (abonementus, regulārus rēķinus). Tev tiks dots
darījumu saraksts JSON formātā ar laukiem: date, description, amount.

Tavs uzdevums:
1. Grupē darījumus pēc tirgotāja/pakalpojuma sniedzēja nosaukuma (ignorējot
   nelielas atšķirības aprakstā, piem., mainīgus rēķinu numurus)
2. Identificē, kuri no tiem atkārtojas regulāri (katru mēnesi, katru nedēļu,
   katru gadu)
3. Aprēķini vidējo summu un nosaki, vai summa laika gaitā mainījusies
   (salīdzini pirmo un pēdējo reizi)
4. Kategorizē katru atrasto abonementu vienā no: streaming, utilities
   (komunālie), insurance (apdrošināšana), telecom (telekomunikācijas),
   subscription_software, food_delivery, gym_fitness, other
5. NEIEKĻAUJ vienreizējus pirkumus, algas ieskaitījumus, pārskaitījumus
   starp lietotāja pašu kontiem, vai nejaušus/neregulārus darījumus

Summas atgriez kā pozitīvus skaitļus (maksājuma lielums), arī tad, ja ievadē
tās ir negatīvas.`;

export const CATEGORIES = [
  "streaming",
  "utilities",
  "insurance",
  "telecom",
  "subscription_software",
  "food_delivery",
  "gym_fitness",
  "other",
] as const;

const isoDate = z
  .string()
  .regex(/^\d{4}-\d{2}-\d{2}$/, "Datumam jābūt formātā YYYY-MM-DD");

const detectedSubscriptionSchema = z.object({
  merchant_name: z.string().min(1),
  category: z.enum(CATEGORIES),
  average_amount: z.number(),
  frequency: z.enum(["monthly", "weekly", "yearly"]),
  occurrences: z.number().int().min(1),
  first_date: isoDate,
  last_date: isoDate,
  price_changed: z.boolean(),
  price_change_percent: z.number(),
  first_amount: z.number(),
  last_amount: z.number(),
});

/**
 * Structured outputs prasa objektu augšējā līmenī, tāpēc masīvs ir ietīts
 * `subscriptions` laukā. Shēmu uzspiež API, tāpēc nav jāpaļaujas uz to, ka
 * modelis "atgriež tikai JSON".
 */
export const analysisSchema = z.object({
  subscriptions: z.array(detectedSubscriptionSchema),
});

export type DetectedSubscription = z.infer<typeof detectedSubscriptionSchema>;

/** Tikai šie lauki tiek sūtīti uz Claude API. */
export interface TransactionForAnalysis {
  date: string;
  description: string;
  amount: number;
}

let client: Anthropic | null = null;

function getClient(): Anthropic {
  if (!env.ANTHROPIC_API_KEY) {
    throw AppError.unavailable(
      "ANTHROPIC_API_KEY nav konfigurēta — AI analīze nav pieejama.",
    );
  }
  client ??= new Anthropic({ apiKey: env.ANTHROPIC_API_KEY });
  return client;
}

/** Pārtulko Anthropic SDK kļūdas mūsu AppError, no konkrētākās uz vispārīgāko. */
export function toAppError(error: unknown, context: string): AppError {
  if (error instanceof AppError) return error;

  if (error instanceof Anthropic.NotFoundError) {
    logger.error(`${context}: modelis nav atrasts`, { model: env.ANTHROPIC_MODEL });
    return AppError.upstream(
      `AI modelis "${env.ANTHROPIC_MODEL}" nav pieejams šai atslēgai`,
    );
  }
  if (error instanceof Anthropic.RateLimitError) {
    return new AppError(
      429,
      "rate_limited",
      "AI serviss ir pārslogots. Mēģini vēlreiz pēc brīža.",
    );
  }
  if (error instanceof Anthropic.AuthenticationError) {
    logger.error(`${context}: nederīga API atslēga`);
    return AppError.unavailable("Nederīga ANTHROPIC_API_KEY");
  }
  // APIConnectionError ir APIError apakšklase — jāpārbauda pirms tās.
  if (error instanceof Anthropic.APIConnectionError) {
    return AppError.upstream("Neizdevās sasniegt AI servisu");
  }
  if (error instanceof Anthropic.APIError) {
    logger.error(`${context}: API kļūda`, { message: error.message });
    return AppError.upstream("AI serviss atgrieza kļūdu");
  }

  logger.error(`${context}: neparedzēta kļūda`, {
    message: error instanceof Error ? error.message : String(error),
  });
  return AppError.upstream("Neizdevās veikt AI analīzi");
}

/**
 * Sagatavo analīzes pieprasījumu.
 *
 * Atdalīts no nosūtīšanas, lai to varētu pārbaudīt ar testiem — jo īpaši to,
 * ka uz API aiziet TIKAI datums, apraksts un summa. Tas ir apsolījums, ko
 * lietotājs redz onboarding ekrānā, tāpēc tam jābūt sargātam ar testu, nevis
 * tikai ar koda lasīšanu.
 */
export function buildAnalysisRequest(transactions: TransactionForAnalysis[]) {
  // Skaidri uzskaitām laukus, nevis padodam objektu tālāk: ja izsaucējs kādreiz
  // atsūtīs vairāk lauku, tie šeit tiks nogriezti.
  const payload = transactions.map((transaction) => ({
    date: transaction.date,
    description: transaction.description,
    amount: transaction.amount,
  }));

  const userPrompt = [
    `Šeit ir darījumu saraksts pēdējiem ${env.TRANSACTION_SYNC_MONTHS} mēnešiem:`,
    "",
    JSON.stringify(payload, null, 2),
    "",
    "Atrodi visus atkārtotos maksājumus un atgriez rezultātu norādītajā formātā.",
  ].join("\n");

  return {
    model: env.ANTHROPIC_MODEL,
    max_tokens: 16000,
    system: SYSTEM_PROMPT,
    thinking: { type: "adaptive" as const },
    messages: [{ role: "user" as const, content: userPrompt }],
    output_config: { format: zodOutputFormat(analysisSchema) },
  };
}

/**
 * Atrod atkārtotos maksājumus darījumu sarakstā.
 * Atgriež tukšu masīvu, ja darījumu nav.
 */
export async function analyzeTransactions(
  transactions: TransactionForAnalysis[],
): Promise<DetectedSubscription[]> {
  if (transactions.length === 0) {
    return [];
  }

  const anthropic = getClient();

  try {
    const message = await anthropic.messages.parse(
      buildAnalysisRequest(transactions),
    );

    if (!message.parsed_output) {
      throw AppError.upstream("AI atbilde neatbilda gaidītajam formātam");
    }

    logger.info("AI analīze pabeigta", {
      transactions: transactions.length,
      detected: message.parsed_output.subscriptions.length,
      inputTokens: message.usage.input_tokens,
      outputTokens: message.usage.output_tokens,
    });

    return message.parsed_output.subscriptions;
  } catch (error) {
    throw toAppError(error, "Darījumu analīze");
  }
}

export type DraftActionType = "cancel" | "negotiate";

/**
 * Sagatavo melnraksta pieprasījumu. Atdalīts no nosūtīšanas, lai promptu un
 * parametrus varētu pārbaudīt ar testiem.
 */
export function buildDraftRequest(
  merchantName: string,
  actionType: DraftActionType,
) {
  const prompt =
    actionType === "cancel"
      ? `Uzraksti īsu, pieklājīgu e-pastu holandiešu vai angļu valodā (atkarībā no
pakalpojuma sniedzēja), lūdzot atcelt abonementu/pakalpojumu ${merchantName}.
Ietver, ka lietotājs vēlas apstiprinājumu par atcelšanu un pēdējo norēķinu
datumu. Turi to īsu (3-4 teikumi) un profesionālu.

Atgriez tikai e-pasta tekstu (temats un ķermenis), bez paskaidrojumiem.`
      : `Uzraksti īsu, pieklājīgu e-pastu holandiešu vai angļu valodā (atkarībā no
pakalpojuma sniedzēja), lūdzot labāku cenu vai piedāvājumu esošajam
pakalpojumam ${merchantName}. Piemini, ka lietotājs apsver pāreju pie
konkurenta, un lūdz konkrētu piedāvājumu. Turi to īsu (3-4 teikumi) un
profesionālu.

Atgriez tikai e-pasta tekstu (temats un ķermenis), bez paskaidrojumiem.`;

  return {
    model: env.ANTHROPIC_MODEL,
    max_tokens: 2000,
    thinking: { type: "adaptive" as const },
    output_config: { effort: "low" as const },
    messages: [{ role: "user" as const, content: prompt }],
  };
}

/**
 * Ģenerē atcelšanas vai pārrunāšanas e-pasta melnrakstu.
 * Melnraksts NETIEK nosūtīts — lietotājs to nokopē un nosūta pats.
 */
export async function generateDraft(
  merchantName: string,
  actionType: DraftActionType,
): Promise<string> {
  const anthropic = getClient();

  try {
    const message = await anthropic.messages.create(
      buildDraftRequest(merchantName, actionType),
    );

    const text = message.content
      .filter((block): block is Anthropic.TextBlock => block.type === "text")
      .map((block) => block.text)
      .join("\n")
      .trim();

    if (!text) {
      throw AppError.upstream("AI neatgrieza melnraksta tekstu");
    }

    return text;
  } catch (error) {
    throw toAppError(error, "Melnraksta ģenerēšana");
  }
}
