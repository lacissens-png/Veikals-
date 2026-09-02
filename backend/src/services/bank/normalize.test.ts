import assert from "node:assert/strict";
import { describe, it } from "node:test";
import {
  extractDescription,
  normalize,
  normalizeAll,
  parseAmount,
  type RawTransaction,
} from "./normalize.js";

describe("parseAmount", () => {
  it("pieņem skaitli", () => {
    assert.equal(parseAmount(12.34), 12.34);
  });

  it("pieņem virkni, kā to atsūta banka", () => {
    assert.equal(parseAmount("12.34"), 12.34);
    assert.equal(parseAmount("-7.5"), -7.5);
  });

  it("atgriež 0 nederīgai vērtībai, nevis NaN", () => {
    // NaN izplatītos cauri visiem aprēķiniem un sabojātu kopsummu.
    assert.equal(parseAmount("nav skaitlis"), 0);
    assert.equal(parseAmount(undefined), 0);
    assert.equal(parseAmount(null), 0);
    assert.equal(parseAmount({}), 0);
    assert.equal(parseAmount(Number.NaN), 0);
    assert.equal(parseAmount(Number.POSITIVE_INFINITY), 0);
  });
});

describe("extractDescription", () => {
  it("dod priekšroku saņēmēja nosaukumam", () => {
    const item: RawTransaction = {
      creditor: { name: "Ziggo Services B.V." },
      debtor: { name: "Kaut kas cits" },
      remittance_information: ["Rēķins 123"],
    };
    assert.equal(extractDescription(item), "Ziggo Services B.V.");
  });

  it("atkāpjas uz maksātāju, tad uz maksājuma informāciju", () => {
    assert.equal(
      extractDescription({ debtor: { name: "Darba devējs" } }),
      "Darba devējs",
    );
    assert.equal(
      extractDescription({ remittance_information: ["Abonements", "augusts"] }),
      "Abonements augusts",
    );
  });

  it("izlaiž tukšas maksājuma informācijas rindas", () => {
    assert.equal(
      extractDescription({ remittance_information: ["", "Netflix", ""] }),
      "Netflix",
    );
  });

  it("nekad neatgriež tukšu aprakstu", () => {
    assert.equal(extractDescription({}), "Nezināms darījums");
    assert.equal(
      extractDescription({ remittance_information: ["", "  "] }),
      "Nezināms darījums",
    );
  });
});

describe("normalize", () => {
  const base: RawTransaction = {
    entry_reference: "ref-1",
    booking_date: "2026-08-26",
    transaction_amount: { amount: "37.22", currency: "EUR" },
    credit_debit_indicator: "DBIT",
    creditor: { name: "Ziggo Services B.V." },
  };

  it("izdevumu padara negatīvu", () => {
    assert.equal(normalize(base).amount, -37.22);
  });

  it("izdevumu padara negatīvu arī tad, ja banka jau atsūta negatīvu", () => {
    // Bankas atšķiras zīmju konvencijā; mums jābūt vienai.
    const item = { ...base, transaction_amount: { amount: "-37.22" } };
    assert.equal(normalize(item).amount, -37.22);
  });

  it("ieskaitījumu padara pozitīvu", () => {
    const item = { ...base, credit_debit_indicator: "CRDT" };
    assert.equal(normalize(item).amount, 37.22);
  });

  it("neatpazītu indikatoru uzskata par izdevumu", () => {
    const item = { ...base, credit_debit_indicator: undefined };
    assert.equal(normalize(item).amount, -37.22);
  });

  it("indikatoru salīdzina neatkarīgi no reģistra", () => {
    const item = { ...base, credit_debit_indicator: "crdt" };
    assert.equal(normalize(item).amount, 37.22);
  });

  it("dod priekšroku entry_reference pār transaction_id", () => {
    const item = { ...base, transaction_id: "tx-9" };
    assert.equal(normalize(item).externalId, "ref-1");
  });

  it("atgriež null, ja bankas ID nav — tad dedublikācija iet pēc satura", () => {
    const { entry_reference, ...withoutRef } = base;
    assert.equal(normalize(withoutRef).externalId, null);
  });

  it("dod priekšroku grāmatošanas datumam un nogriež laiku", () => {
    const item = {
      ...base,
      booking_date: "2026-08-26T10:15:00Z",
      value_date: "2026-08-28",
    };
    assert.equal(normalize(item).date, "2026-08-26");
  });

  it("atkāpjas uz value_date", () => {
    const { booking_date, ...withoutBooking } = base;
    const item = { ...withoutBooking, value_date: "2026-08-28" };
    assert.equal(normalize(item).date, "2026-08-28");
  });

  it("noklusējuma valūta ir EUR", () => {
    const item = { ...base, transaction_amount: { amount: "1.00" } };
    assert.equal(normalize(item).currency, "EUR");
  });

  it("saglabā neapstrādāto atbildi", () => {
    assert.deepEqual(normalize(base).raw, base);
  });
});

describe("normalizeAll", () => {
  it("atmet darījumus bez derīga datuma", () => {
    const items: RawTransaction[] = [
      { booking_date: "2026-08-26", transaction_amount: { amount: "1.00" } },
      { transaction_amount: { amount: "2.00" } },
      { booking_date: "nav", transaction_amount: { amount: "3.00" } },
    ];
    const result = normalizeAll(items);
    assert.equal(result.length, 1);
    assert.equal(result[0]?.date, "2026-08-26");
  });

  it("tukšs saraksts dod tukšu rezultātu", () => {
    assert.deepEqual(normalizeAll([]), []);
  });
});
