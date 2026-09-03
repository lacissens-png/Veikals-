import type { NextFunction, Request, Response } from "express";
import { ZodError } from "zod";
import { AppError } from "../lib/errors.js";
import { logger } from "../lib/logger.js";
import { env } from "../config/env.js";

/** 404 visiem nezināmiem maršrutiem. */
export function notFoundHandler(req: Request, _res: Response, next: NextFunction) {
  next(AppError.notFound(`Maršruts ${req.method} ${req.path} neeksistē`));
}

/**
 * Vienots kļūdu formāts: { error: { code, message, details? } }.
 * Express 5 automātiski pārsūta arī noraidītus async promise'us šurp.
 */
export function errorHandler(
  err: unknown,
  _req: Request,
  res: Response,
  next: NextFunction,
) {
  if (res.headersSent) {
    return next(err);
  }

  if (err instanceof AppError) {
    if (err.status >= 500) {
      logger.error(err.message, { code: err.code, details: err.details });
    }
    res.status(err.status).json({
      error: { code: err.code, message: err.message, details: err.details },
    });
    return;
  }

  if (err instanceof ZodError) {
    res.status(400).json({
      error: {
        code: "validation_error",
        message: "Nederīgi ievades dati",
        details: err.issues.map((issue) => ({
          path: issue.path.join("."),
          message: issue.message,
        })),
      },
    });
    return;
  }

  const message = err instanceof Error ? err.message : String(err);
  logger.error("Neapstrādāta kļūda", {
    message,
    stack: err instanceof Error ? err.stack : undefined,
  });

  res.status(500).json({
    error: {
      code: "internal_error",
      message:
        env.NODE_ENV === "production"
          ? "Iekšēja servera kļūda"
          : message,
    },
  });
}
