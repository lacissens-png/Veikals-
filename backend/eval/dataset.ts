import type { TransactionForAnalysis } from "../src/services/claude.service.js";

/**
 * Novērtējuma datu kopa AI abonementu atpazīšanai.
 *
 * IZCELSME: dati ir pilnībā sintētiski, bet to formas ir atvasinātas no īsta
 * Nīderlandes norēķinu konta izraksta (312 darījumi, 9 mēneši), ko izmantoja
 * projekta validācijai. Nekas no īstajiem darījumiem šeit nav pārnests —
 * pārnesti ir tikai grūtie gadījumi, ko tie atklāja:
 *
 *   1. Ikmēneša apdrošināšana, maksāta NEREGULĀRI (ar nokavējumu, tad divi
 *      uzreiz). Noteikumos balstīts detektors to nepamana; modelim, kas saprot,
 *      ka runa ir par apdrošinātāju ar atkārtotu summu, tas būtu jāatpazīst.
 *      Šis ir galvenais tests, kas atšķir AI no regulāra intervāla meklēšanas.
 *   2. Pirmais rēķins ar pieslēguma izmaksām, kas izskatās pēc cenas krituma.
 *   3. Kredītkartes norēķins — atkārtojas katru mēnesi, bet summa svārstās
 *      trīskārtīgi. Tas NAV abonements.
 *   4. Pārskaitījumi uz paša krājkontu un uz paša kontu ārzemēs.
 *   5. Regulāri pirkumi vienā veikalā ar mainīgām summām.
 */

export type Expectation = "detect" | "ignore" | "either";

export interface ExpectedSubscription {
  /** Fragments, pēc kura meklē modeļa atbildē (reģistrjutīgs nav). */
  merchant: string;
  expectation: Expectation;
  category?: string;
  frequency?: "monthly" | "weekly" | "yearly";
  /** Aptuvenā vidējā summa; pieļaujamā novirze ±15%. */
  averageAmount?: number;
  priceChanged?: boolean;
  /** Kāpēc šis gadījums ir datu kopā. */
  why: string;
}

const tx = (
  date: string,
  description: string,
  amount: number,
): TransactionForAnalysis => ({ date, description, amount });

/** Katra mēneša konkrēts datums deviņu mēnešu periodā. */
function monthly(day: number): string[] {
  const months = [
    "2025-12", "2026-01", "2026-02", "2026-03",
    "2026-04", "2026-05", "2026-06", "2026-07", "2026-08",
  ];
  const dd = String(day).padStart(2, "0");
  return months.map((m) => `${m}-${dd}`);
}

const transactions: TransactionForAnalysis[] = [];

// --- 1. Telekomunikācijas: pirmais rēķins ar pieslēguma izmaksām -------------
// Modelim NAV jāziņo par cenas kritumu — pirmā summa ir vienreizēja.
monthly(26).forEach((date, i) => {
  transactions.push(tx(date, "Kabelnet Verbinding B.V.", i === 0 ? -104.5 : -38.75));
});

// --- 2. Komunālie ar ĪSTU cenas kāpumu --------------------------------------
monthly(25).forEach((date, i) => {
  transactions.push(tx(date, "Stroomdirect Energie B.V", i < 4 ? -135.0 : -152.0));
});

// --- 3. Apdrošināšana, regulāra ---------------------------------------------
monthly(27).forEach((date) => {
  transactions.push(tx(date, "Vastberaad Schadeverzekering NV", -88.4));
});

// --- 4. GRŪTAIS GADĪJUMS: ikmēneša apdrošināšana, maksāta neregulāri ---------
// Atstarpes dienās: 49, 13, 50, 8, 55, 6, 33 — nāk pa pāriem ar garu pauzi.
// Summa ir gandrīz nemainīga, un tirgotājs ir acīmredzams apdrošinātājs.
[
  "2025-12-02", "2026-01-20", "2026-02-02", "2026-03-24",
  "2026-04-01", "2026-05-26", "2026-06-01", "2026-07-04",
].forEach((date) => {
  transactions.push(tx(date, "Zorgpolis Nederland Zorgverzekeraar", -146.5));
});

// --- 5. Straumēšana ar cenas kāpumu -----------------------------------------
monthly(14).forEach((date, i) => {
  transactions.push(tx(date, "STREAMFLIX INTERNATIONAL B.V.", i < 5 ? -13.99 : -15.49));
});

// --- 6. Sporta klubs ---------------------------------------------------------
monthly(1).forEach((date) => {
  transactions.push(tx(date, "FitFabriek Nederland B.V.", -29.99));
});

// --- 7. Maza ikmēneša bankas maksa ------------------------------------------
monthly(5).forEach((date) => {
  transactions.push(tx(date, "Kosten Betaalpakket", -3.95));
});

// --- 8. Gada abonements (divas reizes periodā nav — tikai viena) -------------
// Apzināti: viens gada maksājums NAV pietiekams, lai secinātu atkārtošanos.
transactions.push(tx("2026-02-11", "DomeinHoster Jaarpakket", -89.0));

// --- 9. NAV ABONEMENTS: kredītkartes norēķins -------------------------------
// Atkārtojas katru mēnesi, bet summa svārstās trīskārtīgi.
const creditCard = [645.0, 1180.4, 2500.0, 890.15, 1740.6, 2210.0, 980.25, 1455.8, 2020.0];
monthly(18).forEach((date, i) => {
  transactions.push(tx(date, "Incasso Creditcard Afrekening", -creditCard[i]!));
});

// --- 10. NAV ABONEMENTS: alga ------------------------------------------------
monthly(24).forEach((date, i) => {
  transactions.push(tx(date, "Havenlogistiek Nederland BV", 2192.01 + i * 12.5));
});

// --- 11. NAV ABONEMENTS: pārskaitījums uz paša krājkontu --------------------
monthly(27).forEach((date) => {
  transactions.push(tx(date, "To Oranje spaarrekening S11016258", -25.0));
});

// --- 12. NAV ABONEMENTS: pārskaitījums uz paša kontu ārzemēs ----------------
["2025-12-11", "2026-01-09", "2026-02-14", "2026-03-11", "2026-04-16",
 "2026-05-12", "2026-06-19", "2026-07-15", "2026-08-13"].forEach((date, i) => {
  transactions.push(tx(date, "J. Vandermeer", -[250, 100, 250, 400, 150, 250, 300, 100, 250][i]!));
});

// --- 13. NAV ABONEMENTS: pārtika tajā pašā veikalā, mainīgas summas ---------
["2025-12-05", "2025-12-19", "2026-01-08", "2026-01-22", "2026-02-06",
 "2026-02-20", "2026-03-09", "2026-03-23", "2026-04-07", "2026-04-21",
 "2026-05-11", "2026-05-25", "2026-06-08", "2026-06-22", "2026-07-06",
 "2026-07-20", "2026-08-04", "2026-08-18"].forEach((date, i) => {
  transactions.push(tx(date, "SUPERMARKT 1420 VAALS NLD",
    -[68.41, 41.9, 55.2, 91.34, 37.75, 62.18, 48.9, 73.55, 29.4,
      84.12, 51.66, 39.8, 77.23, 45.9, 58.3, 66.75, 33.2, 88.4][i]!));
});

// --- 14. NAV ABONEMENTS: degviela tajā pašā stacijā -------------------------
["2025-12-14", "2026-01-17", "2026-02-25", "2026-04-02",
 "2026-05-19", "2026-06-27", "2026-08-09"].forEach((date, i) => {
  transactions.push(tx(date, "TANKSTATION KERKRADE NLD",
    -[50.09, 113.6, 62.4, 45.0, 88.75, 51.3, 70.15][i]!));
});

// --- 15. NAV ABONEMENTS: vienreizēji pirkumi --------------------------------
transactions.push(tx("2026-01-24", "Spelconsole Entertainment Europe", -50.0));
transactions.push(tx("2026-03-02", "Meubelwinkel Heerlen NLD", -419.0));
transactions.push(tx("2026-06-13", "Vliegtickets Boeking B.V.", -287.4));

transactions.sort((a, b) => a.date.localeCompare(b.date));

export const evalTransactions = transactions;

export const expectations: ExpectedSubscription[] = [
  {
    merchant: "Kabelnet",
    expectation: "detect",
    category: "telecom",
    frequency: "monthly",
    averageAmount: 38.75,
    priceChanged: false,
    why: "Pirmais rēķins ar pieslēguma izmaksām nav cenas kritums",
  },
  {
    merchant: "Stroomdirect",
    expectation: "detect",
    category: "utilities",
    frequency: "monthly",
    averageAmount: 144.4,
    priceChanged: true,
    why: "Īsts cenas kāpums 135 -> 152",
  },
  {
    merchant: "Vastberaad",
    expectation: "detect",
    category: "insurance",
    frequency: "monthly",
    averageAmount: 88.4,
    why: "Vienkāršs regulārs gadījums",
  },
  {
    merchant: "Zorgpolis",
    expectation: "detect",
    category: "insurance",
    frequency: "monthly",
    averageAmount: 146.5,
    why: "GRŪTAIS: ikmēneša apdrošināšana, maksāta neregulāri — noteikumi to nepamana",
  },
  {
    merchant: "STREAMFLIX",
    expectation: "detect",
    category: "streaming",
    frequency: "monthly",
    averageAmount: 14.66,
    priceChanged: true,
    why: "Straumēšana ar cenas kāpumu",
  },
  {
    merchant: "FitFabriek",
    expectation: "detect",
    category: "gym_fitness",
    frequency: "monthly",
    averageAmount: 29.99,
    why: "Sporta klubs",
  },
  {
    merchant: "Kosten Betaalpakket",
    expectation: "detect",
    frequency: "monthly",
    averageAmount: 3.95,
    why: "Maza summa nedrīkst pazust",
  },
  {
    merchant: "DomeinHoster",
    expectation: "either",
    why: "Viens maksājums — atkārtošanos secināt nevar, abas atbildes pieņemamas",
  },
  {
    merchant: "Creditcard",
    expectation: "ignore",
    why: "Atkārtojas katru mēnesi, bet summa svārstās trīskārtīgi — norēķins, ne abonements",
  },
  {
    merchant: "Havenlogistiek",
    expectation: "ignore",
    why: "Alga — ieskaitījums, ne abonements",
  },
  {
    merchant: "spaarrekening",
    expectation: "ignore",
    why: "Pārskaitījums uz paša krājkontu",
  },
  {
    merchant: "Vandermeer",
    expectation: "ignore",
    why: "Pārskaitījums privātpersonai ar mainīgām summām",
  },
  {
    merchant: "SUPERMARKT",
    expectation: "ignore",
    why: "Regulāri pirkumi vienā veikalā ar mainīgām summām",
  },
  {
    merchant: "TANKSTATION",
    expectation: "ignore",
    why: "Degviela — neregulāra un mainīga",
  },
  {
    merchant: "Spelconsole",
    expectation: "ignore",
    why: "Vienreizējs pirkums",
  },
  {
    merchant: "Meubelwinkel",
    expectation: "ignore",
    why: "Vienreizējs pirkums",
  },
  {
    merchant: "Vliegtickets",
    expectation: "ignore",
    why: "Vienreizējs pirkums",
  },
];
