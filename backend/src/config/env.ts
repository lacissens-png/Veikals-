import "dotenv/config";
import { z } from "zod";

/**
 * Vides mainīgo shēma. Aplikācija neuzsāksies, ja kāds obligāts mainīgais
 * trūkst vai ir nederīgs — labāk skaidra kļūda startā nekā `undefined` vidū.
 */
const envSchema = z.object({
  NODE_ENV: z
    .enum(["development", "test", "production"])
    .default("development"),
  PORT: z.coerce.number().int().positive().default(4000),

  DATABASE_URL: z.string().min(1, "DATABASE_URL ir obligāts"),

  JWT_SECRET: z
    .string()
    .min(32, "JWT_SECRET jābūt vismaz 32 rakstzīmes garam"),
  JWT_EXPIRES_IN: z.string().default("7d"),

  CORS_ORIGIN: z.string().default("*"),

  ANTHROPIC_API_KEY: z.string().optional(),
  ANTHROPIC_MODEL: z.string().default("claude-opus-5"),

  ENABLE_BANKING_API_URL: z
    .string()
    .url()
    .default("https://api.enablebanking.com"),
  ENABLE_BANKING_CLIENT_ID: z.string().optional(),
  ENABLE_BANKING_CLIENT_SECRET: z.string().optional(),
  ENABLE_BANKING_REDIRECT_URI: z
    .string()
    .url()
    .default("http://localhost:4000/api/bank/callback"),
  ENABLE_BANKING_MOCK: z
    .string()
    .default("true")
    .transform((value) => value.toLowerCase() === "true"),

  // ---------------------------------------------------------------------------
  // Gmail (abonementu atrašana e-pastā + krāpšanas brīdinājumi + sūtīšana)
  // ---------------------------------------------------------------------------
  GMAIL_CLIENT_ID: z.string().optional(),
  GMAIL_CLIENT_SECRET: z.string().optional(),
  GMAIL_REDIRECT_URI: z
    .string()
    .url()
    .default("http://localhost:4000/api/email/callback"),
  GMAIL_MOCK: z
    .string()
    .default("true")
    .transform((value) => value.toLowerCase() === "true"),
  /** Cik mēnešus atpakaļ lasīt vēstules. */
  EMAIL_SYNC_MONTHS: z.coerce.number().int().min(1).max(24).default(3),

  // 32 baiti hex (64 rakstzīmes). Ģenerē ar: openssl rand -hex 32
  TOKEN_ENCRYPTION_KEY: z
    .string()
    .regex(/^[0-9a-fA-F]{64}$/, "TOKEN_ENCRYPTION_KEY jābūt 64 hex rakstzīmes")
    .optional(),

  TRANSACTION_SYNC_MONTHS: z.coerce.number().int().min(1).max(24).default(3),
  APP_REDIRECT_URL: z.string().default("abonementi://bank-callback"),
});

/**
 * Mock režīmā bankas tokena nav vispār, tāpēc šifrēšanas atslēga nav vajadzīga
 * un ātrā palaišana strādā bez tās. Ar īstu banku tā ir obligāta — pretējā
 * gadījumā tokens nonāktu datubāzē atklātā tekstā.
 */
/**
 * Acīmredzami neīstā atslēga no .env.example. Izstrādē tā ir ērtība, produkcijā
 * — nopietna kļūda, tāpēc tur to noraidām.
 */
const PLACEHOLDER_KEY = "0000000000000000000000000000000000000000000000000000000000000000";

const envSchemaChecked = envSchema.superRefine((value, ctx) => {
  if (
    value.NODE_ENV === "production" &&
    value.TOKEN_ENCRYPTION_KEY === PLACEHOLDER_KEY
  ) {
    ctx.addIssue({
      code: "custom",
      path: ["TOKEN_ENCRYPTION_KEY"],
      message:
        "produkcijā nedrīkst lietot .env.example izstrādes atslēgu. " +
        "Ģenerē savu ar: openssl rand -hex 32",
    });
  }

  const storesRealTokens = !value.ENABLE_BANKING_MOCK || !value.GMAIL_MOCK;
  if (storesRealTokens && !value.TOKEN_ENCRYPTION_KEY) {
    ctx.addIssue({
      code: "custom",
      path: ["TOKEN_ENCRYPTION_KEY"],
      message:
        "obligāts, kad ENABLE_BANKING_MOCK=false vai GMAIL_MOCK=false — bez tā " +
        "piekļuves tokens tiktu glabāts atklātā tekstā. Ģenerē ar: openssl rand -hex 32",
    });
  }
});

const parsed = envSchemaChecked.safeParse(process.env);

if (!parsed.success) {
  const issues = parsed.error.issues
    .map((issue) => `  - ${issue.path.join(".")}: ${issue.message}`)
    .join("\n");
  throw new Error(`Nederīga vides konfigurācija:\n${issues}`);
}

export const env = parsed.data;

/** true, ja Claude API atslēga ir konfigurēta un AI analīze ir pieejama. */
export const hasAnthropicKey = Boolean(env.ANTHROPIC_API_KEY);

export const corsOrigins =
  env.CORS_ORIGIN === "*"
    ? "*"
    : env.CORS_ORIGIN.split(",").map((origin) => origin.trim());
