/**
 * AI precizitātes novērtējums.
 *
 * Palaiž īsto analīzi (analyzeTransactions) pret marķētu datu kopu un mēra,
 * cik precīzi modelis atpazīst abonementus.
 *
 * ŠIS IZSAUC CLAUDE API UN MAKSĀ NAUDU. CI to nepalaiž.
 *
 *   cd backend && npm run eval
 *
 * Nepieciešama ANTHROPIC_API_KEY vidē vai .env failā.
 */
import {
  analyzeTransactions,
  type DetectedSubscription,
} from "../src/services/claude.service.js";
import {
  evalTransactions,
  expectations,
  type ExpectedSubscription,
} from "./dataset.js";

function matches(
  expected: ExpectedSubscription,
  detected: DetectedSubscription,
): boolean {
  return detected.merchant_name
    .toLowerCase()
    .includes(expected.merchant.toLowerCase());
}

/** Vai summa ir pieļaujamās novirzes robežās. */
function close(actual: number, expected: number, tolerance = 0.15): boolean {
  return Math.abs(actual - expected) <= Math.abs(expected) * tolerance;
}

type Verdict = "OK" | "KĻŪDA" | "BRĪDINĀJUMS";

interface Row {
  label: string;
  verdict: Verdict;
  detail: string;
  why: string;
}

async function main() {
  console.log(`Darījumi datu kopā: ${evalTransactions.length}`);
  console.log(`Marķēti gadījumi:   ${expectations.length}`);
  console.log("\nSūtām uz Claude API...\n");

  const started = Date.now();
  const detected = await analyzeTransactions(evalTransactions);
  const seconds = ((Date.now() - started) / 1000).toFixed(1);

  console.log(`Modelis atgrieza ${detected.length} abonementus (${seconds} s)\n`);

  const rows: Row[] = [];
  const matchedDetections = new Set<DetectedSubscription>();
  let truePositives = 0;
  let falseNegatives = 0;
  let wronglyFlagged = 0;

  for (const expected of expectations) {
    const hit = detected.find((d) => matches(expected, d));
    if (hit) matchedDetections.add(hit);

    if (expected.expectation === "either") {
      rows.push({
        label: expected.merchant,
        verdict: "BRĪDINĀJUMS",
        detail: hit ? "atpazīts (pieņemami)" : "nav atpazīts (pieņemami)",
        why: expected.why,
      });
      continue;
    }

    if (expected.expectation === "detect") {
      if (!hit) {
        falseNegatives += 1;
        rows.push({
          label: expected.merchant,
          verdict: "KĻŪDA",
          detail: "NAV ATPAZĪTS",
          why: expected.why,
        });
        continue;
      }

      truePositives += 1;
      const problems: string[] = [];

      if (expected.category && hit.category !== expected.category) {
        problems.push(`kategorija ${hit.category} != ${expected.category}`);
      }
      if (expected.frequency && hit.frequency !== expected.frequency) {
        problems.push(`biežums ${hit.frequency} != ${expected.frequency}`);
      }
      if (
        expected.averageAmount !== undefined &&
        !close(Math.abs(hit.average_amount), expected.averageAmount)
      ) {
        problems.push(
          `summa ${Math.abs(hit.average_amount).toFixed(2)} != ~${expected.averageAmount.toFixed(2)}`,
        );
      }
      if (
        expected.priceChanged !== undefined &&
        hit.price_changed !== expected.priceChanged
      ) {
        problems.push(`cenas izmaiņa ${hit.price_changed} != ${expected.priceChanged}`);
      }

      rows.push({
        label: expected.merchant,
        verdict: problems.length === 0 ? "OK" : "BRĪDINĀJUMS",
        detail: problems.length === 0 ? "atpazīts pareizi" : problems.join("; "),
        why: expected.why,
      });
      continue;
    }

    // expectation === "ignore"
    if (hit) wronglyFlagged += 1;
    rows.push({
      label: expected.merchant,
      verdict: hit ? "KĻŪDA" : "OK",
      detail: hit ? `NEPAREIZI ATZĪMĒTS ka "${hit.merchant_name}"` : "pareizi izlaists",
      why: expected.why,
    });
  }

  const unexpected = detected.filter((d) => !matchedDetections.has(d));
  const falsePositives = wronglyFlagged + unexpected.length;

  console.log("REZULTĀTI PA GADĪJUMIEM");
  console.log("-".repeat(96));
  for (const row of rows) {
    console.log(`${row.verdict.padEnd(13)}${row.label.padEnd(22)}${row.detail}`);
    console.log(`${" ".repeat(35)}${row.why}`);
  }

  if (unexpected.length > 0) {
    console.log("-".repeat(96));
    console.log("Atpazīti, bet datu kopā nav gaidīti:");
    for (const item of unexpected) {
      console.log(
        `             ${item.merchant_name} (${item.category}, ${item.frequency}, ${Math.abs(item.average_amount).toFixed(2)})`,
      );
    }
  }

  const precision =
    truePositives + falsePositives === 0
      ? 1
      : truePositives / (truePositives + falsePositives);
  const recall =
    truePositives + falseNegatives === 0
      ? 1
      : truePositives / (truePositives + falseNegatives);
  const f1 =
    precision + recall === 0 ? 0 : (2 * precision * recall) / (precision + recall);

  console.log("-".repeat(96));
  console.log(`Pareizi atpazīti (TP):   ${truePositives}`);
  console.log(`Nepareizi atzīmēti (FP): ${falsePositives}`);
  console.log(`Palaisti garām (FN):     ${falseNegatives}`);
  console.log();
  console.log(
    `Precision: ${(precision * 100).toFixed(1)}%   Recall: ${(recall * 100).toFixed(1)}%   F1: ${(f1 * 100).toFixed(1)}%`,
  );

  const failures = rows.filter((r) => r.verdict === "KĻŪDA").length;
  if (failures > 0) {
    console.log(`\n${failures} obligātie gadījumi neizdevās.`);
    process.exitCode = 1;
  } else {
    console.log("\nVisi obligātie gadījumi izdevās.");
  }
}

main().catch((error: unknown) => {
  console.error(
    "\nNovērtējums neizdevās:",
    error instanceof Error ? error.message : error,
  );
  process.exitCode = 1;
});
