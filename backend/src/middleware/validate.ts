import type { NextFunction, Request, Response } from "express";
import type { ZodType } from "zod";

/**
 * Validē `req.body` pret zod shēmu un aizvieto to ar apstrādāto rezultātu.
 * Kļūdas gadījumā ZodError nonāk `errorHandler`, kas atgriež 400.
 */
export function validateBody<T>(schema: ZodType<T>) {
  return (req: Request, _res: Response, next: NextFunction) => {
    const result = schema.safeParse(req.body);
    if (!result.success) {
      next(result.error);
      return;
    }
    req.body = result.data;
    next();
  };
}
