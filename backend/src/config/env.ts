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

  TRANSACTION_SYNC_MONTHS: z.coerce.number().int().min(1).max(24).default(3),
  APP_REDIRECT_URL: z.string().default("abonementi://bank-callback"),
});

const parsed = envSchema.safeParse(process.env);

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
