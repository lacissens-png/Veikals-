import { Router } from "express";
import { z } from "zod";
import { currentUserId, requireAuth } from "../middleware/auth.js";
import { validateBody } from "../middleware/validate.js";
import * as authService from "../services/auth.service.js";

export const accountRouter = Router();

accountRouter.use(requireAuth);

const pushTokenSchema = z.object({
  pushToken: z.string().min(1, "Trūkst push tokena"),
});

/** Expo push tokena reģistrācija paziņojumiem par cenu izmaiņām. */
accountRouter.post(
  "/notifications/token",
  validateBody(pushTokenSchema),
  async (req, res) => {
    const { pushToken } = req.body as z.infer<typeof pushTokenSchema>;
    await authService.savePushToken(currentUserId(req), pushToken);
    res.status(204).send();
  },
);

/** Pilnīga konta un visu datu dzēšana (SettingsScreen). */
accountRouter.delete("/me", async (req, res) => {
  await authService.deleteAccount(currentUserId(req));
  res.status(204).send();
});
