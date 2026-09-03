import { Router } from "express";
import { currentUserId, requireAuth } from "../middleware/auth.js";
import { prisma } from "../lib/prisma.js";
import * as transactionsService from "../services/transactions.service.js";

export const transactionsRouter = Router();

/** Ielādē darījumus no bankas un saglabā tos datubāzē. */
transactionsRouter.post("/sync", requireAuth, async (req, res) => {
  const result = await transactionsService.syncTransactions(currentUserId(req));
  res.json(result);
});

/** Darījumu saraksts (noder atkļūdošanai un detaļu skatam). */
transactionsRouter.get("/", requireAuth, async (req, res) => {
  const limit = Math.min(Number(req.query.limit ?? 100) || 100, 500);

  const transactions = await prisma.transaction.findMany({
    where: { bankConnection: { userId: currentUserId(req) } },
    orderBy: { date: "desc" },
    take: limit,
    select: {
      id: true,
      date: true,
      description: true,
      amount: true,
      currency: true,
    },
  });

  res.json({
    transactions: transactions.map((transaction) => ({
      ...transaction,
      date: transaction.date.toISOString().slice(0, 10),
      amount: Number(transaction.amount),
    })),
  });
});
