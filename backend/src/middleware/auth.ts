import type { NextFunction, Request, Response } from "express";
import jwt from "jsonwebtoken";
import { env } from "../config/env.js";
import { AppError } from "../lib/errors.js";

export interface JwtPayload {
  sub: string;
  email: string;
}

declare global {
  // eslint-disable-next-line @typescript-eslint/no-namespace
  namespace Express {
    interface Request {
      /** Aizpilda `requireAuth`. Pieejams tikai aizsargātajos maršrutos. */
      user?: JwtPayload;
    }
  }
}

/** Izsniedz JWT pēc veiksmīgas reģistrācijas/pieteikšanās. */
export function signToken(payload: JwtPayload): string {
  return jwt.sign(payload, env.JWT_SECRET, {
    expiresIn: env.JWT_EXPIRES_IN,
  } as jwt.SignOptions);
}

/** Pieprasa derīgu `Authorization: Bearer <token>` galveni. */
export function requireAuth(req: Request, _res: Response, next: NextFunction) {
  const header = req.headers.authorization;

  if (!header?.startsWith("Bearer ")) {
    next(AppError.unauthorized("Trūkst Authorization: Bearer galvenes"));
    return;
  }

  const token = header.slice("Bearer ".length).trim();

  try {
    const decoded = jwt.verify(token, env.JWT_SECRET) as JwtPayload;
    req.user = { sub: decoded.sub, email: decoded.email };
    next();
  } catch {
    next(AppError.unauthorized("Nederīgs vai beidzies token"));
  }
}

/** Īsceļš lietotāja ID iegūšanai aizsargātā maršrutā. */
export function currentUserId(req: Request): string {
  if (!req.user?.sub) {
    throw AppError.unauthorized();
  }
  return req.user.sub;
}
