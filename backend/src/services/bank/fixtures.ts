import type { BankTransaction } from "./types.js";

/**
 * Testa darījumi mock režīmam (ENABLE_BANKING_MOCK=true).
 *
 * Ļauj izmēģināt pilnu plūsmu — sinhronizācija, AI analīze, atcelšanas
 * melnraksts — bez Enable Banking līguma un bez īstiem bankas datiem.
 * Dati atdarina tipisku Nīderlandes norēķinu kontu:
 *   - regulāri abonementi (Ziggo, Netflix, Spotify, Basic-Fit)
 *   - komunālie un apdrošināšana (Essent, Zilveren Kruis)
 *   - viens abonements ar cenas kāpumu (Netflix 13.99 -> 15.49)
 *   - troksnis, kas AI nav jāatpazīst kā abonements (veikali, alga, pārskaitījumi)
 */

interface Recurring {
  description: string;
  /** Summa vai [vecā, jaunā] — otrā stājas spēkā no `priceChangeFrom` mēneša. */
  amount: number | [number, number];
  dayOfMonth: number;
  priceChangeFromMonthIndex?: number;
}

const RECURRING: Recurring[] = [
  { description: "Ziggo Services B.V.", amount: 37.22, dayOfMonth: 26 },
  { description: "Essent Retail Energie B.V", amount: 150.0, dayOfMonth: 25 },
  {
    description: "NETFLIX INTERNATIONAL B.V.",
    amount: [13.99, 15.49],
    dayOfMonth: 14,
    // Sākas otrajā mēnesī no perioda sākuma, lai kāpums vienmēr iekrīt
    // pagātnē un ir redzams analīzē neatkarīgi no palaišanas datuma.
    priceChangeFromMonthIndex: 1,
  },
  { description: "Spotify AB", amount: 10.99, dayOfMonth: 8 },
  { description: "Basic-Fit Nederland B.V.", amount: 29.99, dayOfMonth: 1 },
  { description: "Zilveren Kruis Zorgverzekering", amount: 138.5, dayOfMonth: 3 },
  { description: "Vodafone Libertel B.V.", amount: 22.5, dayOfMonth: 18 },
];

/** Vienreizēji darījumi — AI tos NEDRĪKST atzīmēt kā abonementus. */
const ONE_OFF: Array<{ description: string; amount: number; dayOfMonth: number }> = [
  { description: "ALBERT HEIJN 1234 AMSTERDAM", amount: -68.41, dayOfMonth: 5 },
  { description: "ALBERT HEIJN 1234 AMSTERDAM", amount: -41.9, dayOfMonth: 19 },
  { description: "SALARIS WERKGEVER B.V.", amount: 3200.0, dayOfMonth: 24 },
  { description: "Eigen spaarrekening overboeking", amount: -500.0, dayOfMonth: 24 },
  { description: "BOL.COM B.V. bestelling 8871234", amount: -119.95, dayOfMonth: 11 },
  { description: "NS GROEP IZ VSA", amount: -14.6, dayOfMonth: 7 },
];

function isoDate(year: number, month: number, day: number): string {
  // Pieķeram mēnešus, kuros nav 29.-31. datuma.
  const lastDay = new Date(Date.UTC(year, month + 1, 0)).getUTCDate();
  const safeDay = Math.min(day, lastDay);
  const date = new Date(Date.UTC(year, month, safeDay));
  return date.toISOString().slice(0, 10);
}

/**
 * Ģenerē darījumus par pēdējiem `months` mēnešiem, skaitot no šodienas atpakaļ.
 * Vienmēr atgriež svaigus datumus, lai analīze strādā neatkarīgi no tā, kad
 * projekts tiek palaists.
 */
export function generateMockTransactions(months: number): BankTransaction[] {
  const now = new Date();
  const transactions: BankTransaction[] = [];

  for (let monthIndex = months - 1; monthIndex >= 0; monthIndex -= 1) {
    const cursor = new Date(
      Date.UTC(now.getUTCFullYear(), now.getUTCMonth() - monthIndex, 1),
    );
    const year = cursor.getUTCFullYear();
    const month = cursor.getUTCMonth();
    const monthsFromStart = months - 1 - monthIndex;

    for (const item of RECURRING) {
      const amount = Array.isArray(item.amount)
        ? monthsFromStart >= (item.priceChangeFromMonthIndex ?? 0)
          ? item.amount[1]
          : item.amount[0]
        : item.amount;

      const date = isoDate(year, month, item.dayOfMonth);
      if (new Date(date) > now) continue;

      transactions.push({
        externalId: `mock-${item.description.slice(0, 12)}-${date}`.replace(/\s+/g, "-"),
        date,
        description: item.description,
        amount: -Math.abs(amount),
        currency: "EUR",
        raw: { source: "mock", recurring: true },
      });
    }

    for (const item of ONE_OFF) {
      const date = isoDate(year, month, item.dayOfMonth);
      if (new Date(date) > now) continue;

      transactions.push({
        externalId: `mock-${item.description.slice(0, 12)}-${date}`.replace(/\s+/g, "-"),
        date,
        description: item.description,
        amount: item.amount,
        currency: "EUR",
        raw: { source: "mock", recurring: false },
      });
    }
  }

  return transactions.sort((a, b) => a.date.localeCompare(b.date));
}
