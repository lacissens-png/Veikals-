import {
  createCipheriv,
  createDecipheriv,
  randomBytes,
  timingSafeEqual,
} from "node:crypto";
import { env } from "../config/env.js";
import { AppError } from "./errors.js";
import { logger } from "./logger.js";

/**
 * Bankas piekļuves tokenu šifrēšana glabāšanai datubāzē.
 *
 * AES-256-GCM ar Node iebūvēto crypto — bez jaunām atkarībām. GCM dod ne tikai
 * šifrēšanu, bet arī autentifikācijas tagu, tāpēc bojāts vai pamainīts ieraksts
 * tiek noraidīts, nevis atšifrēts par blēņām.
 *
 * Formāts: v1:<iv>:<tag>:<ciphertext>, katra daļa base64.
 * Prefikss ļauj vēlāk nomainīt algoritmu, nesalaužot esošos ierakstus.
 */

const VERSION = "v1";
const ALGORITHM = "aes-256-gcm";
const IV_BYTES = 12; // GCM standarta nonce garums
const KEY_BYTES = 32;

function loadKey(): Buffer {
  if (!env.TOKEN_ENCRYPTION_KEY) {
    throw AppError.unavailable(
      "TOKEN_ENCRYPTION_KEY nav konfigurēta — bankas tokenu nevar droši saglabāt. " +
        "Ģenerē ar: openssl rand -hex 32",
    );
  }

  const key = Buffer.from(env.TOKEN_ENCRYPTION_KEY, "hex");
  if (key.length !== KEY_BYTES) {
    throw AppError.unavailable(
      `TOKEN_ENCRYPTION_KEY jābūt ${KEY_BYTES} baiti hex formātā (${KEY_BYTES * 2} rakstzīmes)`,
    );
  }
  return key;
}

/** Vai virkne izskatās pēc mūsu šifrētā formāta. */
export function isEncrypted(value: string): boolean {
  return value.startsWith(`${VERSION}:`);
}

/** Šifrē tokenu. `null` paliek `null` — mock režīmā tokena nav. */
export function encryptToken(plaintext: string | null): string | null {
  if (plaintext === null) return null;

  const key = loadKey();
  const iv = randomBytes(IV_BYTES);
  const cipher = createCipheriv(ALGORITHM, key, iv);

  const ciphertext = Buffer.concat([
    cipher.update(plaintext, "utf8"),
    cipher.final(),
  ]);

  return [
    VERSION,
    iv.toString("base64"),
    cipher.getAuthTag().toString("base64"),
    ciphertext.toString("base64"),
  ].join(":");
}

/**
 * Atšifrē tokenu.
 *
 * Ieraksti bez `v1:` prefiksa ir no laika pirms šifrēšanas ieviešanas — tie tiek
 * atgriezti neskarti ar brīdinājumu žurnālā. Bez tā esošie savienojumi pēc
 * izvietošanas pārstātu darboties. Jauni ieraksti vienmēr tiek šifrēti.
 */
export function decryptToken(stored: string | null): string | null {
  if (stored === null) return null;

  if (!isEncrypted(stored)) {
    logger.warn(
      "Bankas tokens datubāzē ir atklātā tekstā (no laika pirms šifrēšanas). " +
        "Tas tiks šifrēts nākamajā savienojuma atjaunošanā.",
    );
    return stored;
  }

  const parts = stored.split(":");
  if (parts.length !== 4) {
    throw AppError.upstream("Bojāts šifrētais tokens datubāzē");
  }

  const [, ivPart, tagPart, dataPart] = parts as [string, string, string, string];

  try {
    const decipher = createDecipheriv(
      ALGORITHM,
      loadKey(),
      Buffer.from(ivPart, "base64"),
    );
    decipher.setAuthTag(Buffer.from(tagPart, "base64"));

    return Buffer.concat([
      decipher.update(Buffer.from(dataPart, "base64")),
      decipher.final(),
    ]).toString("utf8");
  } catch (error) {
    if (error instanceof AppError) throw error;
    // GCM autentifikācijas taga neatbilstība nonāk šeit: ieraksts ir pamainīts
    // vai atslēga ir cita. Abos gadījumos to nedrīkst lietot.
    logger.error("Neizdevās atšifrēt bankas tokenu", {
      message: error instanceof Error ? error.message : String(error),
    });
    throw AppError.upstream(
      "Neizdevās atšifrēt bankas tokenu — savieno banku no jauna",
    );
  }
}

/** Salīdzina divus noslēpumus laika ziņā droši. Noder testos un turpmāk. */
export function secretsMatch(a: string, b: string): boolean {
  const bufferA = Buffer.from(a, "utf8");
  const bufferB = Buffer.from(b, "utf8");
  if (bufferA.length !== bufferB.length) return false;
  return timingSafeEqual(bufferA, bufferB);
}
