import { randomBytes } from "node:crypto";
import { encryptToken } from "../lib/crypto.js";
import { AppError } from "../lib/errors.js";
import { logger } from "../lib/logger.js";
import { prisma } from "../lib/prisma.js";
import { safeReturnUrl } from "../lib/redirect.js";
import { bankProvider } from "./bank/enableBanking.service.js";

/**
 * Sāk bankas OAuth plūsmu: izveido 'pending' savienojumu ar nejaušu `state`
 * un atgriež saiti, uz kuru frontend aizved lietotāju.
 */
export async function startConnection(userId: string, returnUrl?: string) {
  const state = randomBytes(24).toString("hex");

  const { authorizationUrl } = await bankProvider.startAuthorization(state);

  const connection = await prisma.bankConnection.create({
    data: {
      userId,
      provider: bankProvider.name,
      status: "pending",
      authState: state,
      returnUrl: safeReturnUrl(returnUrl),
    },
    select: { id: true },
  });

  logger.info("Bankas autorizācija sākta", {
    userId,
    bankConnectionId: connection.id,
  });

  return { bankConnectionId: connection.id, authorizationUrl, state };
}

/**
 * Apstrādā atgriešanos no bankas. `state` sasaista callback ar konkrēto
 * savienojumu un pasargā no CSRF — callback nav autentificēts ar JWT, jo
 * lietotāju atsūta banka.
 */
export async function completeConnection(code: string, state: string) {
  const connection = await prisma.bankConnection.findUnique({
    where: { authState: state },
  });

  if (!connection) {
    throw AppError.badRequest("Nezināms vai jau izmantots `state` parametrs");
  }

  if (connection.status === "active") {
    throw AppError.conflict("Šis savienojums jau ir pabeigts");
  }

  try {
    const result = await bankProvider.exchangeCallback(code);

    const updated = await prisma.bankConnection.update({
      where: { id: connection.id },
      data: {
        status: "active",
        providerAccountId: result.accountId,
        sessionId: result.sessionId,
        // Tokens datubāzē nonāk tikai šifrēts.
        accessToken: encryptToken(result.accessToken),
        tokenExpiresAt: result.expiresAt,
        connectedAt: new Date(),
        // `state` derīgs vienu reizi.
        authState: null,
      },
      select: { id: true, userId: true, providerAccountId: true, returnUrl: true },
    });

    logger.info("Banka savienota", {
      userId: updated.userId,
      bankConnectionId: updated.id,
    });

    return updated;
  } catch (error) {
    await prisma.bankConnection.update({
      where: { id: connection.id },
      data: { status: "error", authState: null },
    });
    throw error;
  }
}

export async function listConnections(userId: string) {
  const connections = await prisma.bankConnection.findMany({
    where: { userId },
    orderBy: { connectedAt: "desc" },
    select: {
      id: true,
      provider: true,
      status: true,
      connectedAt: true,
      lastSyncedAt: true,
      providerAccountId: true,
    },
  });

  return connections.map((connection) => ({
    ...connection,
    // Konta identifikators tiek maskēts — pilns IBAN nav vajadzīgs UI.
    providerAccountId: connection.providerAccountId
      ? `•••${connection.providerAccountId.slice(-4)}`
      : null,
  }));
}

/** Bankas atvienošana (SettingsScreen). Darījumi tiek dzēsti kaskādē. */
export async function disconnect(userId: string, connectionId: string) {
  const connection = await prisma.bankConnection.findFirst({
    where: { id: connectionId, userId },
    select: { id: true },
  });

  if (!connection) {
    throw AppError.notFound("Bankas savienojums nav atrasts");
  }

  await prisma.bankConnection.delete({ where: { id: connection.id } });
  logger.info("Banka atvienota", { userId, bankConnectionId: connectionId });
}
