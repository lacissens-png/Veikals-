import { Router } from "express";
import { z } from "zod";
import { pathParam } from "../lib/http.js";
import { currentUserId, requireAuth } from "../middleware/auth.js";
import { validateBody } from "../middleware/validate.js";
import * as emailService from "../services/email.service.js";
import * as subscriptionsService from "../services/subscriptions.service.js";

export const subscriptionsRouter = Router();

subscriptionsRouter.use(requireAuth);

/** Nosūta darījumus uz Claude API un saglabā atrastos abonementus. */
subscriptionsRouter.post("/analyze", async (req, res) => {
  const result = await subscriptionsService.analyzeAndStore(currentUserId(req));
  res.json(result);
});

/** Abonementu saraksts ar kopsavilkumu (DashboardScreen). */
subscriptionsRouter.get("/", async (req, res) => {
  res.json(await subscriptionsService.listSubscriptions(currentUserId(req)));
});

/** Viena abonementa detaļas ar maksājumu vēsturi. */
subscriptionsRouter.get("/:id", async (req, res) => {
  res.json(
    await subscriptionsService.getSubscriptionDetail(
      currentUserId(req),
      pathParam(req, "id"),
    ),
  );
});

const flagSchema = z.object({ isFlaggedUnwanted: z.boolean() });

/** "Atzīmēt kā nevēlamu". */
subscriptionsRouter.patch("/:id", validateBody(flagSchema), async (req, res) => {
  const { isFlaggedUnwanted } = req.body as z.infer<typeof flagSchema>;
  res.json(
    await subscriptionsService.setUnwantedFlag(
      currentUserId(req),
      pathParam(req, "id"),
      isFlaggedUnwanted,
    ),
  );
});

/** Atcelšanas e-pasta melnraksts. Netiek nosūtīts — lietotājs to kopē pats. */
subscriptionsRouter.post("/:id/draft-cancel", async (req, res) => {
  res.status(201).json(
    await subscriptionsService.createDraftAction(
      currentUserId(req),
      pathParam(req, "id"),
      "cancel",
    ),
  );
});

/** Pārrunāšanas e-pasta melnraksts ("Pārrunāt" poga detaļu ekrānā). */
subscriptionsRouter.post("/:id/draft-negotiate", async (req, res) => {
  res.status(201).json(
    await subscriptionsService.createDraftAction(
      currentUserId(req),
      pathParam(req, "id"),
      "negotiate",
    ),
  );
});

/**
 * Nosūta melnrakstu no lietotāja pasta. Prasa aktīvu e-pasta savienojumu —
 * adresāts tiek ņemts no paša tirgotāja vēstulēm pastkastītē.
 */
subscriptionsRouter.post("/drafts/:draftId/send", async (req, res) => {
  res.json(
    await emailService.sendDraft(currentUserId(req), pathParam(req, "draftId")),
  );
});

const draftStatusSchema = z.object({
  status: z.enum(["copied", "dismissed"]),
});

/** Atzīmē melnrakstu kā nokopētu vai atmestu. */
subscriptionsRouter.patch(
  "/drafts/:draftId",
  validateBody(draftStatusSchema),
  async (req, res) => {
    const { status } = req.body as z.infer<typeof draftStatusSchema>;
    res.json(
      await subscriptionsService.updateDraftStatus(
        currentUserId(req),
        pathParam(req, "draftId"),
        status,
      ),
    );
  },
);
