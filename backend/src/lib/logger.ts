/**
 * Minimāls strukturēts logeris. MVP posmā pietiek ar JSON uz stdout —
 * Railway/Render to savāc automātiski.
 */
type Level = "debug" | "info" | "warn" | "error";

function write(level: Level, message: string, meta?: Record<string, unknown>) {
  const line = {
    ts: new Date().toISOString(),
    level,
    message,
    ...(meta ?? {}),
  };
  const serialized = JSON.stringify(line);
  if (level === "error") {
    console.error(serialized);
  } else {
    console.log(serialized);
  }
}

export const logger = {
  debug: (message: string, meta?: Record<string, unknown>) =>
    write("debug", message, meta),
  info: (message: string, meta?: Record<string, unknown>) =>
    write("info", message, meta),
  warn: (message: string, meta?: Record<string, unknown>) =>
    write("warn", message, meta),
  error: (message: string, meta?: Record<string, unknown>) =>
    write("error", message, meta),
};
