import { createApp } from "./app.js";
import { env } from "./config/env.js";
import { logger } from "./lib/logger.js";
import { disconnectPrisma } from "./lib/prisma.js";

const app = createApp();

const server = app.listen(env.PORT, () => {
  logger.info("Backend palaists", {
    port: env.PORT,
    env: env.NODE_ENV,
    bankMock: env.ENABLE_BANKING_MOCK,
  });
});

/** Korekta apstāšanās, lai Railway/Render deploy nepārrautu savienojumus. */
async function shutdown(signal: string) {
  logger.info("Apstājas", { signal });
  server.close(() => {
    void disconnectPrisma().finally(() => process.exit(0));
  });
  setTimeout(() => process.exit(1), 10_000).unref();
}

process.on("SIGTERM", () => void shutdown("SIGTERM"));
process.on("SIGINT", () => void shutdown("SIGINT"));
