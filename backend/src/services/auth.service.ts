import bcrypt from "bcryptjs";
import { signToken } from "../middleware/auth.js";
import { AppError } from "../lib/errors.js";
import { prisma } from "../lib/prisma.js";

const BCRYPT_ROUNDS = 12;

export interface AuthResult {
  token: string;
  user: { id: string; email: string; createdAt: Date };
}

export async function register(
  email: string,
  password: string,
): Promise<AuthResult> {
  const normalizedEmail = email.trim().toLowerCase();

  const existing = await prisma.user.findUnique({
    where: { email: normalizedEmail },
    select: { id: true },
  });

  if (existing) {
    throw AppError.conflict("Lietotājs ar šādu e-pastu jau eksistē");
  }

  const passwordHash = await bcrypt.hash(password, BCRYPT_ROUNDS);

  const user = await prisma.user.create({
    data: { email: normalizedEmail, passwordHash },
    select: { id: true, email: true, createdAt: true },
  });

  return { token: signToken({ sub: user.id, email: user.email }), user };
}

export async function login(
  email: string,
  password: string,
): Promise<AuthResult> {
  const normalizedEmail = email.trim().toLowerCase();

  const user = await prisma.user.findUnique({
    where: { email: normalizedEmail },
  });

  // Viena un tā pati kļūda abos gadījumos, lai neatklātu, kuri e-pasti ir reģistrēti.
  const invalid = AppError.unauthorized("Nepareizs e-pasts vai parole");

  if (!user) {
    // Salīdzinām pret fiktīvu hash, lai atbildes laiks neatklāj konta esamību.
    await bcrypt.compare(password, "$2a$12$invalidinvalidinvalidinvalidinvalidinvalidinvalidinva");
    throw invalid;
  }

  const matches = await bcrypt.compare(password, user.passwordHash);
  if (!matches) {
    throw invalid;
  }

  return {
    token: signToken({ sub: user.id, email: user.email }),
    user: { id: user.id, email: user.email, createdAt: user.createdAt },
  };
}

/** Push tokena reģistrācija paziņojumiem par cenu izmaiņām. */
export async function savePushToken(userId: string, pushToken: string) {
  await prisma.user.update({
    where: { id: userId },
    data: { pushToken },
  });
}

/** Pilnīga konta un visu saistīto datu dzēšana (SettingsScreen). */
export async function deleteAccount(userId: string) {
  // Kaskādes dzēšana shēmā aizvāc bank_connections, transactions,
  // subscriptions un draft_actions.
  await prisma.user.delete({ where: { id: userId } });
}
