import { Router } from "express";
import { z } from "zod";
import { env } from "../config/env.js";
import { AppError } from "../lib/errors.js";
import { withStatus } from "../lib/redirect.js";
import { pathParam } from "../lib/http.js";
import { currentUserId, requireAuth } from "../middleware/auth.js";
import { validateBody } from "../middleware/validate.js";
import * as emailService from "../services/email.service.js";

export const emailRouter = Router();

/** Sāk Gmail OAuth plūsmu. */
const connectSchema = z.object({ redirectUrl: z.string().optional() });

emailRouter.post("/connect", requireAuth, validateBody(connectSchema), async (req, res) => {
  const { redirectUrl } = req.body as z.infer<typeof connectSchema>;
  res.status(201).json(
    await emailService.startConnection(currentUserId(req), redirectUrl),
  );
});

const callbackSchema = z.object({
  code: z.string().min(1),
  state: z.string().min(1),
});

/** Google atgriešanās punkts — bez JWT, savienojumu identificē `state`. */
emailRouter.get("/callback", async (req, res) => {
  const parsed = callbackSchema.safeParse(req.query);

  if (!parsed.success) {
    const reason = typeof req.query.error === "string" ? req.query.error : "invalid_request";
    res.redirect(withStatus(env.APP_REDIRECT_URL, { status: "error", source: "email", reason }));
    return;
  }

  try {
    const connection = await emailService.completeConnection(parsed.data.code, parsed.data.state);
    res.redirect(
      withStatus(connection.returnUrl ?? env.APP_REDIRECT_URL, {
        status: "success",
        source: "email",
        connectionId: connection.id,
      }),
    );
  } catch (error) {
    const reason = error instanceof AppError ? error.code : "unknown_error";
    res.redirect(withStatus(env.APP_REDIRECT_URL, { status: "error", source: "email", reason }));
  }
});

emailRouter.get("/connections", requireAuth, async (req, res) => {
  res.json({ connections: await emailService.listConnections(currentUserId(req)) });
});

emailRouter.delete("/connections/:id", requireAuth, async (req, res) => {
  await emailService.disconnect(currentUserId(req), pathParam(req, "id"));
  res.status(204).send();
});

/** Ielādē vēstules un palaiž AI analīzi. */
emailRouter.post("/sync", requireAuth, async (req, res) => {
  res.json(await emailService.syncAndAnalyze(currentUserId(req)));
});

/** Brīdinājumi un gaidāmie maksājumi. */
emailRouter.get("/findings", requireAuth, async (req, res) => {
  res.json(await emailService.listFindings(currentUserId(req)));
});

const statusSchema = z.object({
  status: z.enum(["acknowledged", "dismissed"]),
});

emailRouter.patch(
  "/findings/:id",
  requireAuth,
  validateBody(statusSchema),
  async (req, res) => {
    const { status } = req.body as z.infer<typeof statusSchema>;
    res.json(
      await emailService.setFindingStatus(currentUserId(req), pathParam(req, "id"), status),
    );
  },
);
