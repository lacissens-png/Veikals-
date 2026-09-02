import type { BankTransaction } from "./types.js";

/**
 * Enable Banking atbilžu normalizēšana. Tīras funkcijas bez I/O, konfigurācijas
 * un blakusefektiem — atsevišķi no HTTP klienta, lai tās var pārbaudīt ar testiem.
 *
 * Šis ir produkcijas ceļš: mock režīms to apiet pilnībā, tāpēc testi ir vienīgais,
 * kas to izpilda, kamēr nav īsta PSD2 savienojuma.
 */

/** Viena darījuma neapstrādātā forma, kā to atgriež Enable Banking. */
export interface RawTransaction {
  entry_reference?: string;
  transaction_id?: string;
  booking_date?: string;
  value_date?: string;
  transaction_amount?: { amount?: string | number; currency?: string };
  credit_debit_indicator?: string;
  remittance_information?: string[];
  creditor?: { name?: string };
  debtor?: { name?: string };
}

/** Summas nāk gan kā skaitlis, gan kā virkne ("12.34"). */
export function parseAmount(value: unknown): number {
  if (typeof value === "number") {
    return Number.isFinite(value) ? value : 0;
  }
  if (typeof value === "string") {
    const parsed = Number.parseFloat(value);
    return Number.isFinite(parsed) ? parsed : 0;
  }
  return 0;
}

/** Izvelk cilvēkam saprotamu aprakstu no dažādiem iespējamiem laukiem. */
export function extractDescription(item: RawTransaction): string {
  const remittance = item.remittance_information?.filter(Boolean).join(" ").trim();

  return (
    item.creditor?.name ??
    item.debtor?.name ??
    (remittance && remittance.length > 0 ? remittance : "Nezināms darījums")
  );
}

/**
 * Pārveido bankas darījumu mūsu iekšējā formā.
 *
 * Zīmju konvencija: izdevumi vienmēr ir negatīvi, ieskaitījumi pozitīvi,
 * neatkarīgi no tā, kādu zīmi atsūta banka. Visa pārējā aplikācija paļaujas
 * uz šo vienoto konvenciju.
 */
export function normalize(item: RawTransaction): BankTransaction {
  const rawAmount = parseAmount(item.transaction_amount?.amount);
  const isDebit = (item.credit_debit_indicator ?? "DBIT").toUpperCase() === "DBIT";
  const date = (item.booking_date ?? item.value_date ?? "").slice(0, 10);

  return {
    externalId: item.entry_reference ?? item.transaction_id ?? null,
    date,
    description: extractDescription(item),
    amount: isDebit ? -Math.abs(rawAmount) : Math.abs(rawAmount),
    currency: item.transaction_amount?.currency ?? "EUR",
    raw: item,
  };
}

/** Atmet darījumus bez derīga datuma — bez tā tie ir nelietojami analīzē. */
export function normalizeAll(items: RawTransaction[]): BankTransaction[] {
  return items.map(normalize).filter((item) => item.date.length === 10);
}
