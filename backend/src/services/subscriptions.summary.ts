/**
 * Kopsavilkuma aprēķini abonementu pārskatam.
 *
 * Tīra matemātika bez datubāzes — tie ir skaitļi, ko lietotājs redz kā
 * "Kopā mēnesī", tāpēc tie ir pārbaudāmi atsevišķi no datu ielādes.
 */

/** Reizinātājs, ar ko biežums tiek pārrēķināts uz mēneša izmaksām. */
const MONTHLY_FACTOR: Record<string, number> = {
  weekly: 52 / 12,
  monthly: 1,
  yearly: 1 / 12,
};

/** Viens abonements tikai ar tiem laukiem, kas vajadzīgi kopsavilkumam. */
export interface SummarizableSubscription {
  averageAmount: number | null;
  frequency: string | null;
  category: string | null;
  isFlaggedUnwanted: boolean;
  priceChangeDetected: boolean;
}

export interface CategoryTotal {
  category: string;
  monthlyTotal: number;
  count: number;
}

export interface SubscriptionSummary {
  monthlyTotal: number;
  yearlyTotal: number;
  count: number;
  flaggedCount: number;
  priceChangeCount: number;
  byCategory: CategoryTotal[];
}

export function round(value: number): number {
  return Math.round(value * 100) / 100;
}

/**
 * Pārrēķina abonementa summu uz mēneša izmaksām.
 * Nezināms biežums tiek uzskatīts par ikmēneša — tā ir biežākā forma, un
 * labāk parādīt aptuvenu summu nekā nulli.
 */
export function monthlyCost(
  amount: number | null,
  frequency: string | null,
): number {
  if (amount === null || !Number.isFinite(amount)) return 0;
  return amount * (MONTHLY_FACTOR[frequency ?? "monthly"] ?? 1);
}

/** Kopsummas un sadalījums pa kategorijām, sakārtots no dārgākās. */
export function summarize(
  subscriptions: SummarizableSubscription[],
): SubscriptionSummary {
  const byCategory = new Map<string, { monthlyTotal: number; count: number }>();
  let monthlyTotal = 0;

  for (const subscription of subscriptions) {
    const monthly = monthlyCost(
      subscription.averageAmount,
      subscription.frequency,
    );
    monthlyTotal += monthly;

    const category = subscription.category ?? "other";
    const bucket = byCategory.get(category) ?? { monthlyTotal: 0, count: 0 };
    bucket.monthlyTotal += monthly;
    bucket.count += 1;
    byCategory.set(category, bucket);
  }

  return {
    monthlyTotal: round(monthlyTotal),
    yearlyTotal: round(monthlyTotal * 12),
    count: subscriptions.length,
    flaggedCount: subscriptions.filter((item) => item.isFlaggedUnwanted).length,
    priceChangeCount: subscriptions.filter((item) => item.priceChangeDetected)
      .length,
    byCategory: [...byCategory.entries()]
      .map(([category, bucket]) => ({
        category,
        monthlyTotal: round(bucket.monthlyTotal),
        count: bucket.count,
      }))
      .sort((a, b) => b.monthlyTotal - a.monthlyTotal),
  };
}
