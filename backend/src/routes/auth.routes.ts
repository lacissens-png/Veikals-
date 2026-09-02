import { Router } from "express";
import { z } from "zod";
import { currentUserId, requireAuth } from "../middleware/auth.js";
import { validateBody } from "../middleware/validate.js";
import * as authService from "../services/auth.service.js";

export const authRouter = Router();

const credentialsSchema = z.object({
  email: z.email("Nederīgs e-pasta formāts"),
  password: z
    .string()
    .min(8, "Parolei jābūt vismaz 8 rakstzīmes garai")
    .max(128, "Parole ir pārāk gara"),
});

authRouter.post(
  "/register",
  validateBody(credentialsSchema),
  async (req, res) => {
    const { email, password } = req.body as z.infer<typeof credentialsSchema>;
    const result = await authService.register(email, password);
    res.status(201).json(result);
  },
);

authRouter.post("/login", validateBody(credentialsSchema), async (req, res) => {
  const { email, password } = req.body as z.infer<typeof credentialsSchema>;
  const result = await authService.login(email, password);
  res.json(result);
});

/** Pašreizējais lietotājs — frontend to izmanto sesijas pārbaudei startā. */
authRouter.get("/me", requireAuth, (req, res) => {
  res.json({ id: currentUserId(req), email: req.user?.email });
});
