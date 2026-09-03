import assert from "node:assert/strict";
import { describe, it } from "node:test";
import {
  monthlyCost,
  summarize,
  type SummarizableSubscription,
} from "./subscriptions.summary.js";

function sub(
  overrides: Partial<SummarizableSubscription> = {},
): SummarizableSubscription {
  return {
    averageAmount: 10,
    frequency: "monthly",
    category: "streaming",
    isFlaggedUnwanted: false,
    priceChangeDetected: false,
    ...overrides,
  };
}

describe("monthlyCost", () => {
  it("ikmēneša summa paliek nemainīga", () => {
    assert.equal(monthlyCost(37.22, "monthly"), 37.22);
  });

  it("gada summa tiek dalīta ar 12", () => {
    assert.equal(monthlyCost(120, "yearly"), 10);
  });

  it("nedēļas summa tiek reizināta ar 52/12", () => {
    assert.equal(monthlyCost(12, "weekly"), 52);
  });

  it("nezināmu biežumu uzskata par ikmēneša", () => {
    assert.equal(monthlyCost(10, "quarterly"), 10);
    assert.equal(monthlyCost(10, null), 10);
  });

  it("trūkstoša summa dod 0, nevis NaN", () => {
    assert.equal(monthlyCost(null, "monthly"), 0);
    assert.equal(monthlyCost(Number.NaN, "monthly"), 0);
  });
});

describe("summarize", () => {
  it("tukšs saraksts dod nulles, nevis NaN", () => {
    const summary = summarize([]);
    assert.equal(summary.monthlyTotal, 0);
    assert.equal(summary.yearlyTotal, 0);
    assert.equal(summary.count, 0);
    assert.deepEqual(summary.byCategory, []);
  });

  it("saskaita mēneša kopsummu pāri dažādiem biežumiem", () => {
    const summary = summarize([
      sub({ averageAmount: 37.22, frequency: "monthly" }),
      sub({ averageAmount: 120, frequency: "yearly" }),
    ]);
    assert.equal(summary.monthlyTotal, 47.22);
  });

  it("gada kopsumma ir mēneša kopsumma reiz 12", () => {
    const summary = summarize([sub({ averageAmount: 15.5 })]);
    assert.equal(summary.yearlyTotal, 186);
  });

  it("noapaļo līdz diviem cipariem", () => {
    // 10 EUR nedēļā = 43.3333... EUR mēnesī
    const summary = summarize([sub({ averageAmount: 10, frequency: "weekly" })]);
    assert.equal(summary.monthlyTotal, 43.33);
  });

  it("grupē pa kategorijām un kārto no dārgākās", () => {
    const summary = summarize([
      sub({ averageAmount: 15, category: "streaming" }),
      sub({ averageAmount: 138.5, category: "insurance" }),
      sub({ averageAmount: 10, category: "streaming" }),
    ]);

    assert.deepEqual(summary.byCategory, [
      { category: "insurance", monthlyTotal: 138.5, count: 1 },
      { category: "streaming", monthlyTotal: 25, count: 2 },
    ]);
  });

  it("abonementus bez kategorijas liek pie 'other'", () => {
    const summary = summarize([sub({ category: null })]);
    assert.equal(summary.byCategory[0]?.category, "other");
  });

  it("skaita atzīmētos un tos, kam mainījusies cena", () => {
    const summary = summarize([
      sub({ isFlaggedUnwanted: true }),
      sub({ isFlaggedUnwanted: true, priceChangeDetected: true }),
      sub(),
    ]);

    assert.equal(summary.count, 3);
    assert.equal(summary.flaggedCount, 2);
    assert.equal(summary.priceChangeCount, 1);
  });

  it("kategoriju kopsummas saskaitās ar galveno kopsummu", () => {
    const summary = summarize([
      sub({ averageAmount: 37.22, category: "telecom" }),
      sub({ averageAmount: 29.99, category: "gym_fitness" }),
      sub({ averageAmount: 138.5, category: "insurance" }),
    ]);

    const sumOfCategories = summary.byCategory.reduce(
      (total, item) => total + item.monthlyTotal,
      0,
    );
    assert.equal(Math.round(sumOfCategories * 100) / 100, summary.monthlyTotal);
  });
});
