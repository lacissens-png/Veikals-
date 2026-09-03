import express from "express";
import cors from "cors";
import { corsOrigins, env, hasAnthropicKey } from "./config/env.js";
import { errorHandler, notFoundHandler } from "./middleware/errorHandler.js";
import { apiRouter } from "./routes/index.js";

export function createApp() {
  const app = express();

  app.use(cors({ origin: corsOrigins }));
  app.use(express.json({ limit: "1mb" }));
  app.disable("x-powered-by");

  /** Veselības pārbaude — Railway/Render to izmanto deploy statusam. */
  app.get("/health", (_req, res) => {
    res.json({
      status: "ok",
      env: env.NODE_ENV,
      bankMock: env.ENABLE_BANKING_MOCK,
      aiConfigured: hasAnthropicKey,
    });
  });

  app.use("/api", apiRouter);

  app.use(notFoundHandler);
  app.use(errorHandler);

  return app;
}
