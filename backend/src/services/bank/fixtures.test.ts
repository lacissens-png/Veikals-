import assert from "node:assert/strict";
import { describe, it } from "node:test";
import { generateMockTransactions } from "./fixtures.js";

/**
 * Testa dati ir tas, uz kā balstās visa MVP demonstrācija un AI precizitātes
 * novērtējums, tāpēc to īpašības ir jāsargā ar testiem.
 */
describe("generateMockTransactions", () => {
  const transactions = generateMockTransactions(3);

  it("ģenerē darījumus", () => {
    assert.ok(transactions.length > 0);
  });

  it("nesatur nākotnes datumus", () => {
    // Nākotnes darījumi izskatītos pēc kļūdas un sabojātu analīzi.
    const today = new Date().toISOString().slice(0, 10);
    for (const transaction of transactions) {
      assert.ok(
        transaction.date <= today,
        `${transaction.description} ir nākotnē: ${transaction.date}`,
      );
    }
  });

  it("visiem datumiem ir formāts YYYY-MM-DD", () => {
    for (const transaction of transactions) {
      assert.match(transaction.date, /^\d{4}-\d{2}-\d{2}$/);
    }
  });

  it("ir sakārtots hronoloģiski", () => {
    const dates = transactions.map((item) => item.date);
    assert.deepEqual(dates, [...dates].sort());
  });

  it("bankas ID ir unikāli", () => {
    // Dedublikācija balstās uz unique (bank_connection_id, external_id).
    // Dublikāti klusi pazustu sinhronizācijā.
    const ids = transactions.map((item) => item.externalId);
    assert.equal(new Set(ids).size, ids.length);
  });

  it("satur atkārtotus maksājumus, ko AI ir jāatpazīst", () => {
    const ziggo = transactions.filter((item) =>
      item.description.includes("Ziggo"),
    );
    assert.ok(ziggo.length >= 2, "Ziggo jāparādās vairākos mēnešos");
    assert.ok(ziggo.every((item) => item.amount === -37.22));
  });

  it("satur abonementu ar cenas izmaiņu", () => {
    const netflix = transactions
      .filter((item) => item.description.includes("NETFLIX"))
      .map((item) => Math.abs(item.amount));

    assert.ok(netflix.length >= 2, "Netflix jāparādās vairākos mēnešos");
    assert.ok(
      new Set(netflix).size > 1,
      `cenai jāmainās, bet visas summas ir vienādas: ${netflix.join(", ")}`,
    );
    assert.ok(
      Math.max(...netflix) > Math.min(...netflix),
      "jābūt gan vecajai, gan jaunajai cenai",
    );
  });

  it("satur troksni, ko AI nedrīkst atzīmēt kā abonementu", () => {
    const hasSalary = transactions.some((item) =>
      item.description.includes("SALARIS"),
    );
    const hasGroceries = transactions.some((item) =>
      item.description.includes("ALBERT HEIJN"),
    );
    assert.ok(hasSalary, "jābūt algas ieskaitījumam");
    assert.ok(hasGroceries, "jābūt vienreizējiem pirkumiem");
  });

  it("algas ieskaitījums ir pozitīvs, abonementi negatīvi", () => {
    const salary = transactions.find((item) =>
      item.description.includes("SALARIS"),
    );
    assert.ok(salary && salary.amount > 0);

    const spotify = transactions.find((item) =>
      item.description.includes("Spotify"),
    );
    assert.ok(spotify && spotify.amount < 0);
  });

  it("ievēro pieprasīto mēnešu skaitu", () => {
    const oneMonth = generateMockTransactions(1);
    const threeMonths = generateMockTransactions(3);
    assert.ok(threeMonths.length > oneMonth.length);
  });
});
