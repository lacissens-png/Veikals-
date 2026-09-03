import { env } from "../config/env.js";
import { decryptToken } from "../lib/crypto.js";
import { AppError } from "../lib/errors.js";
import { logger } from "../lib/logger.js";
import { prisma } from "../lib/prisma.js";
import { bankProvider } from "./bank/enableBanking.service.js";
import type { BankTransaction } from "./bank/types.js";

export interface SyncResult {
  bankConnectionId: string;
  fetched: number;
  inserted: number;
  skipped: number;
}

/** Datums, no kura ielādēt darījumus (TRANSACTION_SYNC_MONTHS atpakaļ). */
function syncFromDate(): Date {
  const date = new Date();
  date.setUTCMonth(date.getUTCMonth() - env.TRANSACTION_SYNC_MONTHS);
  date.setUTCHours(0, 0, 0, 0);
  return date;
}

/**
 * Ielādē darījumus no bankas un saglabā tos datubāzē.
 *
 * Dedublikācija: ja bankas darījumam ir `external_id`, unique indekss
 * (bank_connection_id, external_id) neļauj to ievietot divreiz. Darījumiem bez
 * ID salīdzinām datumu + aprakstu + summu.
 */
export async function syncTransactions(userId: string): Promise<SyncResult> {
  const connection = await prisma.bankConnection.findFirst({
    where: { userId, status: "active" },
    orderBy: { connectedAt: "desc" },
  });

  if (!connection) {
    throw AppError.badRequest(
      "Nav aktīva bankas savienojuma. Vispirms savieno kontu caur /api/bank/connect.",
    );
  }

  if (!connection.providerAccountId) {
    throw AppError.badRequest("Bankas savienojumam nav piesaistīta konta");
  }

  const from = syncFromDate();
  const fetched = await bankProvider.fetchTransactions({
    accountId: connection.providerAccountId,
    accessToken: decryptToken(connection.accessToken),
    from,
  });

  const inserted = await storeTransactions(connection.id, fetched);

  await prisma.bankConnection.update({
    where: { id: connection.id },
    data: { lastSyncedAt: new Date() },
  });

  logger.info("Darījumi sinhronizēti", {
    bankConnectionId: connection.id,
    fetched: fetched.length,
    inserted,
  });

  return {
    bankConnectionId: connection.id,
    fetched: fetched.length,
    inserted,
    skipped: fetched.length - inserted,
  };
}

async function storeTransactions(
  bankConnectionId: string,
  transactions: BankTransaction[],
): Promise<number> {
  if (transactions.length === 0) return 0;

  const existing = await prisma.transaction.findMany({
    where: { bankConnectionId },
    select: { externalId: true, date: true, description: true, amount: true },
  });

  // Divi atslēgu komplekti: bankas ID tiem, kam tas ir, un
  // datums+apraksts+summa pārējiem.
  const seenExternalIds = new Set(
    existing
      .map((item) => item.externalId)
      .filter((id): id is string => id !== null),
  );
  const seenComposite = new Set(
    existing
      .filter((item) => item.externalId === null)
      .map(
        (item) =>
          `${item.date.toISOString().slice(0, 10)}|${item.description}|${item.amount.toFixed(2)}`,
      ),
  );

  const rows = [];

  for (const transaction of transactions) {
    if (transaction.externalId !== null) {
      if (seenExternalIds.has(transaction.externalId)) continue;
      seenExternalIds.add(transaction.externalId);
    } else {
      const key = `${transaction.date}|${transaction.description}|${transaction.amount.toFixed(2)}`;
      if (seenComposite.has(key)) continue;
      seenComposite.add(key);
    }

    rows.push({
      bankConnectionId,
      externalId: transaction.externalId,
      date: new Date(`${transaction.date}T00:00:00.000Z`),
      description: transaction.description,
      amount: transaction.amount.toFixed(2),
      currency: transaction.currency,
      rawData: transaction.raw as object,
    });
  }

  if (rows.length === 0) return 0;

  // skipDuplicates aizsargā arī pret vienlaicīgu sinhronizāciju no diviem
  // pieprasījumiem — unique (bank_connection_id, external_id) tur robežu.
  const result = await prisma.transaction.createMany({
    data: rows,
    skipDuplicates: true,
  });

  return result.count;
}

/** Visi lietotāja darījumi analīzes periodā, jaunākie pēdējie. */
export async function listUserTransactions(userId: string) {
  return prisma.transaction.findMany({
    where: {
      bankConnection: { userId },
      date: { gte: syncFromDate() },
    },
    orderBy: { date: "asc" },
    select: { date: true, description: true, amount: true },
  });
}
