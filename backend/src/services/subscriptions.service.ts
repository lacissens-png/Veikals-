import { Prisma } from "../generated/prisma/client.js";
import { AppError } from "../lib/errors.js";
import { logger } from "../lib/logger.js";
import { prisma } from "../lib/prisma.js";
import {
  analyzeTransactions,
  generateDraft,
  type DetectedSubscription,
  type DraftActionType,
} from "./claude.service.js";
import { notifyPriceChanges, type PriceChangeNotice } from "./notifications.service.js";
import { listUserTransactions } from "./transactions.service.js";

/** Reizinātājs, ar ko biežums tiek pārrēķināts uz mēneša izmaksām. */
const MONTHLY_FACTOR: Record<string, number> = {
  weekly: 52 / 12,
  monthly: 1,
  yearly: 1 / 12,
};

function toDate(value: string): Date {
  return new Date(`${value}T00:00:00.000Z`);
}

function toDecimal(value: number): Prisma.Decimal {
  return new Prisma.Decimal(value.toFixed(2));
}

/**
 * Palaiž AI analīzi pār lietotāja darījumiem un saglabā rezultātus.
 *
 * Upsert pēc (user_id, merchant_name) nozīmē, ka atkārtota analīze atjauno
 * esošos ierakstus, nevis rada dublikātus, un saglabā lietotāja manuālo
 * `is_flagged_unwanted` atzīmi.
 */
export async function analyzeAndStore(userId: string) {
  const transactions = await listUserTransactions(userId);

  if (transactions.length === 0) {
    throw AppError.badRequest(
      "Nav darījumu, ko analizēt. Vispirms palaid /api/transactions/sync.",
    );
  }

  // Uz Claude API aiziet tikai šie trīs lauki — bez personas datiem.
  const detected = await analyzeTransactions(
    transactions.map((transaction) => ({
      date: transaction.date.toISOString().slice(0, 10),
      description: transaction.description,
      amount: Number(transaction.amount),
    })),
  );

  const priceChanges: PriceChangeNotice[] = [];

  for (const item of detected) {
    const previous = await prisma.subscription.findUnique({
      where: {
        subscription_user_merchant: { userId, merchantName: item.merchant_name },
      },
      select: { averageAmount: true, priceChangeDetected: true },
    });

    await upsertSubscription(userId, item);

    // Paziņojam tikai par JAUNU cenas izmaiņu, lai katra analīze neatkārto
    // to pašu paziņojumu.
    const isNewChange =
      item.price_changed &&
      (previous === null || previous.priceChangeDetected === false);

    if (isNewChange) {
      priceChanges.push({
        merchantName: item.merchant_name,
        oldAmount: Math.abs(item.first_amount),
        newAmount: Math.abs(item.last_amount),
        changePercent: item.price_change_percent,
      });
    }
  }

  if (priceChanges.length > 0) {
    const user = await prisma.user.findUnique({
      where: { id: userId },
      select: { pushToken: true },
    });
    await notifyPriceChanges(user?.pushToken ?? null, priceChanges);
  }

  logger.info("Abonementu analīze saglabāta", {
    userId,
    detected: detected.length,
    priceChanges: priceChanges.length,
  });

  return listSubscriptions(userId);
}

async function upsertSubscription(userId: string, item: DetectedSubscription) {
  const values = {
    category: item.category,
    averageAmount: toDecimal(Math.abs(item.average_amount)),
    frequency: item.frequency,
    firstSeen: toDate(item.first_date),
    lastSeen: toDate(item.last_date),
    priceChangeDetected: item.price_changed,
    priceChangePercent: toDecimal(item.price_change_percent),
    occurrences: item.occurrences,
    firstAmount: toDecimal(Math.abs(item.first_amount)),
    lastAmount: toDecimal(Math.abs(item.last_amount)),
  };

  await prisma.subscription.upsert({
    where: {
      subscription_user_merchant: { userId, merchantName: item.merchant_name },
    },
    // `isFlaggedUnwanted` apzināti nav update sarakstā — tā ir lietotāja
    // manuāla atzīme, ko AI analīze nedrīkst pārrakstīt.
    update: values,
    create: { userId, merchantName: item.merchant_name, ...values },
  });
}

export interface SubscriptionSummary {
  monthlyTotal: number;
  yearlyTotal: number;
  count: number;
  flaggedCount: number;
  priceChangeCount: number;
  byCategory: Array<{ category: string; monthlyTotal: number; count: number }>;
}

/** Abonementu saraksts kopā ar kopsavilkumu DashboardScreen vajadzībām. */
export async function listSubscriptions(userId: string) {
  const subscriptions = await prisma.subscription.findMany({
    where: { userId },
    orderBy: [{ averageAmount: "desc" }, { merchantName: "asc" }],
  });

  const byCategory = new Map<string, { monthlyTotal: number; count: number }>();
  let monthlyTotal = 0;

  for (const subscription of subscriptions) {
    const monthly = monthlyCost(subscription.averageAmount, subscription.frequency);
    monthlyTotal += monthly;

    const category = subscription.category ?? "other";
    const bucket = byCategory.get(category) ?? { monthlyTotal: 0, count: 0 };
    bucket.monthlyTotal += monthly;
    bucket.count += 1;
    byCategory.set(category, bucket);
  }

  const summary: SubscriptionSummary = {
    monthlyTotal: round(monthlyTotal),
    yearlyTotal: round(monthlyTotal * 12),
    count: subscriptions.length,
    flaggedCount: subscriptions.filter((item) => item.isFlaggedUnwanted).length,
    priceChangeCount: subscriptions.filter((item) => item.priceChangeDetected).length,
    byCategory: [...byCategory.entries()]
      .map(([category, bucket]) => ({
        category,
        monthlyTotal: round(bucket.monthlyTotal),
        count: bucket.count,
      }))
      .sort((a, b) => b.monthlyTotal - a.monthlyTotal),
  };

  return { summary, subscriptions: subscriptions.map(serialize) };
}

/** Viena abonementa detaļas ar maksājumu vēsturi (SubscriptionDetailScreen). */
export async function getSubscriptionDetail(userId: string, id: string) {
  const subscription = await prisma.subscription.findFirst({
    where: { id, userId },
    include: {
      draftActions: { orderBy: { createdAt: "desc" }, take: 5 },
    },
  });

  if (!subscription) {
    throw AppError.notFound("Abonements nav atrasts");
  }

  // Vēsture: darījumi, kuru apraksts satur tirgotāja nosaukumu. Vienkāršs
  // MVP risinājums — precīzāku sasaisti dos atsevišķa transaction_id kolonna.
  const history = await prisma.transaction.findMany({
    where: {
      bankConnection: { userId },
      description: {
        contains: subscription.merchantName.split(" ")[0] ?? subscription.merchantName,
        mode: "insensitive",
      },
    },
    orderBy: { date: "desc" },
    take: 24,
    select: { id: true, date: true, description: true, amount: true, currency: true },
  });

  return {
    ...serialize(subscription),
    monthlyCost: round(monthlyCost(subscription.averageAmount, subscription.frequency)),
    drafts: subscription.draftActions.map((draft) => ({
      id: draft.id,
      actionType: draft.actionType,
      draftText: draft.draftText,
      status: draft.status,
      createdAt: draft.createdAt,
    })),
    history: history.map((transaction) => ({
      id: transaction.id,
      date: transaction.date.toISOString().slice(0, 10),
      description: transaction.description,
      amount: Number(transaction.amount),
      currency: transaction.currency,
    })),
  };
}

/** "Atzīmēt kā nevēlamu" (MVP funkcija #7). */
export async function setUnwantedFlag(
  userId: string,
  id: string,
  isFlaggedUnwanted: boolean,
) {
  const existing = await prisma.subscription.findFirst({
    where: { id, userId },
    select: { id: true },
  });

  if (!existing) {
    throw AppError.notFound("Abonements nav atrasts");
  }

  const updated = await prisma.subscription.update({
    where: { id },
    data: { isFlaggedUnwanted },
  });

  return serialize(updated);
}

/**
 * Ģenerē un saglabā atcelšanas/pārrunāšanas melnrakstu.
 * Melnraksts NETIEK nosūtīts — lietotājs to nokopē un nosūta pats.
 */
export async function createDraftAction(
  userId: string,
  id: string,
  actionType: DraftActionType,
) {
  const subscription = await prisma.subscription.findFirst({
    where: { id, userId },
    select: { id: true, merchantName: true },
  });

  if (!subscription) {
    throw AppError.notFound("Abonements nav atrasts");
  }

  const draftText = await generateDraft(subscription.merchantName, actionType);

  const draft = await prisma.draftAction.create({
    data: { subscriptionId: subscription.id, actionType, draftText },
  });

  return {
    id: draft.id,
    subscriptionId: draft.subscriptionId,
    merchantName: subscription.merchantName,
    actionType: draft.actionType,
    draftText: draft.draftText,
    status: draft.status,
    createdAt: draft.createdAt,
  };
}

/** Atzīmē melnrakstu kā nokopētu vai atmestu. */
export async function updateDraftStatus(
  userId: string,
  draftId: string,
  status: "copied" | "dismissed",
) {
  const draft = await prisma.draftAction.findFirst({
    where: { id: draftId, subscription: { userId } },
    select: { id: true },
  });

  if (!draft) {
    throw AppError.notFound("Melnraksts nav atrasts");
  }

  const updated = await prisma.draftAction.update({
    where: { id: draftId },
    data: { status },
  });

  return { id: updated.id, status: updated.status };
}

function monthlyCost(
  amount: Prisma.Decimal | null,
  frequency: string | null,
): number {
  if (!amount) return 0;
  const factor = MONTHLY_FACTOR[frequency ?? "monthly"] ?? 1;
  return Number(amount) * factor;
}

function round(value: number): number {
  return Math.round(value * 100) / 100;
}

type SubscriptionRow = Awaited<
  ReturnType<typeof prisma.subscription.findFirstOrThrow>
>;

/** Decimal -> number, Date -> YYYY-MM-DD, lai frontend nav jāparsē Prisma tipi. */
function serialize(subscription: SubscriptionRow) {
  return {
    id: subscription.id,
    merchantName: subscription.merchantName,
    category: subscription.category,
    averageAmount: subscription.averageAmount
      ? Number(subscription.averageAmount)
      : null,
    frequency: subscription.frequency,
    firstSeen: subscription.firstSeen?.toISOString().slice(0, 10) ?? null,
    lastSeen: subscription.lastSeen?.toISOString().slice(0, 10) ?? null,
    priceChangeDetected: subscription.priceChangeDetected,
    priceChangePercent: subscription.priceChangePercent
      ? Number(subscription.priceChangePercent)
      : null,
    isFlaggedUnwanted: subscription.isFlaggedUnwanted,
    occurrences: subscription.occurrences,
    firstAmount: subscription.firstAmount ? Number(subscription.firstAmount) : null,
    lastAmount: subscription.lastAmount ? Number(subscription.lastAmount) : null,
    updatedAt: subscription.updatedAt,
  };
}
