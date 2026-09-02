import type { Request } from "express";
import { AppError } from "./errors.js";

/**
 * Nolasa maršruta parametru. Express tipi to atzīmē kā iespējami `undefined`,
 * tāpēc šis palīgs to pārbauda vienuviet, nevis katrā maršrutā atsevišķi.
 */
export function pathParam(req: Request, name: string): string {
  const value = req.params[name];

  if (typeof value !== "string" || value.length === 0) {
    throw AppError.badRequest(`Trūkst maršruta parametra \`${name}\``);
  }

  return value;
}
