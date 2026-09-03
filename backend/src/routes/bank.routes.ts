import { Router } from "express";
import { z } from "zod";
import { env } from "../config/env.js";
import { withStatus } from "../lib/redirect.js";
import { AppError } from "../lib/errors.js";
import { pathParam } from "../lib/http.js";
import { currentUserId, requireAuth } from "../middleware/auth.js";
import { validateBody } from "../middleware/validate.js";
import * as bankService from "../services/bank.service.js";

export const bankRouter = Router();

/** Sāk Enable Banking OAuth plūsmu. */
const connectSchema = z.object({
  /** Kurp lietotne grib atgriezties. Expo Go un būvēta lietotne atšķiras. */
  redirectUrl: z.string().optional(),
});

bankRouter.post("/connect", requireAuth, validateBody(connectSchema), async (req, res) => {
  const { redirectUrl } = req.body as z.infer<typeof connectSchema>;
  res.status(201).json(
    await bankService.startConnection(currentUserId(req), redirectUrl),
  );
});

const callbackSchema = z.object({
  code: z.string().min(1, "Trūkst `code` parametra"),
  state: z.string().min(1, "Trūkst `state` parametra"),
});

/**
 * Bankas atgriešanās punkts. Šeit nav JWT — lietotāju atsūta banka, tāpēc
 * savienojumu identificē `state`. Pēc apstrādes lietotājs tiek novirzīts
 * atpakaļ uz aplikāciju (deep link).
 */
bankRouter.get("/callback", async (req, res) => {
  const parsed = callbackSchema.safeParse(req.query);

  if (!parsed.success) {
    const error = typeof req.query.error === "string" ? req.query.error : "invalid_request";
    res.redirect(withStatus(env.APP_REDIRECT_URL, { status: "error", reason: error }));
    return;
  }

  try {
    const connection = await bankService.completeConnection(
      parsed.data.code,
      parsed.data.state,
    );
    res.redirect(
      withStatus(connection.returnUrl ?? env.APP_REDIRECT_URL, {
        status: "success",
        connectionId: connection.id,
      }),
    );
  } catch (error) {
    const reason = error instanceof AppError ? error.code : "unknown_error";
    res.redirect(withStatus(env.APP_REDIRECT_URL, { status: "error", reason }));
  }
});

bankRouter.get("/connections", requireAuth, async (req, res) => {
  res.json({ connections: await bankService.listConnections(currentUserId(req)) });
});

/** Bankas atvienošana — dzēš savienojumu un tā darījumus. */
bankRouter.delete("/connections/:id", requireAuth, async (req, res) => {
  await bankService.disconnect(currentUserId(req), pathParam(req, "id"));
  res.status(204).send();
});
