import { env } from "../config/env.js";
import { logger } from "./logger.js";

/**
 * Kurp novirzīt lietotāju pēc OAuth.
 *
 * Lietotne pati pasaka savu atgriešanās adresi, jo tā atšķiras pēc vides:
 * Expo Go lieto `exp://192.168.1.10:8081/--/...`, būvēta lietotne — `abonementi://`.
 * Bez tā plūsma uz telefona apstātos, jo `abonementi://` Expo Go nav reģistrēts.
 *
 * Adrese nāk no klienta, tāpēc tā tiek pārbaudīta: atļautas ir tikai lietotnes
 * pašas shēmas. Citādi tas būtu atvērtais novirzītājs, ko var izmantot
 * pīķšķerēšanai.
 */
const ALLOWED_SCHEMES = ["exp", "exps", "abonementi", "http", "https"];

export function safeReturnUrl(requested: string | null | undefined): string {
  if (!requested) return env.APP_REDIRECT_URL;

  let parsed: URL;
  try {
    parsed = new URL(requested);
  } catch {
    logger.warn("Nederīga atgriešanās adrese — lietojam noklusējumu");
    return env.APP_REDIRECT_URL;
  }

  const scheme = parsed.protocol.replace(":", "").toLowerCase();

  if (!ALLOWED_SCHEMES.includes(scheme)) {
    logger.warn("Neatļauta atgriešanās shēma — lietojam noklusējumu", { scheme });
    return env.APP_REDIRECT_URL;
  }

  // http/https atļauts tikai lokālai izstrādei (Expo web).
  if ((scheme === "http" || scheme === "https") && !isLocal(parsed.hostname)) {
    logger.warn("Ārēja atgriešanās adrese noraidīta", { host: parsed.hostname });
    return env.APP_REDIRECT_URL;
  }

  return requested;
}

function isLocal(hostname: string): boolean {
  return (
    hostname === "localhost" ||
    hostname === "127.0.0.1" ||
    /^10\./.test(hostname) ||
    /^192\.168\./.test(hostname) ||
    /^172\.(1[6-9]|2\d|3[01])\./.test(hostname)
  );
}

/** Pievieno statusa parametrus atgriešanās adresei. */
export function withStatus(
  base: string,
  params: Record<string, string>,
): string {
  const separator = base.includes("?") ? "&" : "?";
  const query = new URLSearchParams(params).toString();
  return `${base}${separator}${query}`;
}
