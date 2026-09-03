import assert from "node:assert/strict";
import { describe, it } from "node:test";
import { buildSearchQuery, extractPlainText, parseSender } from "./gmail.service.js";

describe("buildSearchQuery", () => {
  it("ierobežo lasīšanu ar datumu", () => {
    const query = buildSearchQuery(new Date("2026-06-01T00:00:00Z"));
    assert.ok(query.includes("after:2026/06/01"));
  });

  it("meklē gan holandiešu, gan angļu terminus", () => {
    const query = buildSearchQuery(new Date());
    for (const term of ["abonnement", "subscription", "factuur", "invoice"]) {
      assert.ok(query.includes(`"${term}"`), `trūkst ${term}`);
    }
  });

  it("nekad nelasa visu pastkastīti", () => {
    // Bez OR filtra vaicājums atgrieztu visu — tas ir tas, no kā izvairāmies.
    const query = buildSearchQuery(new Date());
    assert.ok(query.includes(" OR "));
    assert.ok(query.startsWith("after:"));
  });
});

describe("parseSender", () => {
  it("atdala vārdu no adreses", () => {
    assert.deepEqual(parseSender("Ziggo <klantenservice@ziggo.nl>"), {
      name: "Ziggo",
      address: "klantenservice@ziggo.nl",
    });
  });

  it("apstrādā pēdiņas nosaukumā", () => {
    assert.deepEqual(parseSender('"ING Bank" <info@ing.nl>'), {
      name: "ING Bank",
      address: "info@ing.nl",
    });
  });

  it("apstrādā kailu adresi", () => {
    assert.deepEqual(parseSender("noreply@essent.nl"), {
      name: "noreply@essent.nl",
      address: "noreply@essent.nl",
    });
  });

  it("adresi pieraksta mazajiem burtiem", () => {
    assert.equal(parseSender("X <INFO@ZIGGO.NL>").address, "info@ziggo.nl");
  });
});

describe("extractPlainText", () => {
  const encode = (text: string) =>
    Buffer.from(text, "utf8").toString("base64").replace(/\+/g, "-").replace(/\//g, "_");

  it("izvelk vienkāršu teksta ķermeni", () => {
    const payload = { mimeType: "text/plain", body: { data: encode("Sveiks") } };
    assert.equal(extractPlainText(payload), "Sveiks");
  });

  it("atrod teksta daļu multipart vēstulē", () => {
    const payload = {
      mimeType: "multipart/alternative",
      parts: [
        { mimeType: "text/html", body: { data: encode("<p>HTML</p>") } },
        { mimeType: "text/plain", body: { data: encode("Teksts") } },
      ],
    };
    assert.equal(extractPlainText(payload), "Teksts");
  });

  it("atkāpjas uz HTML bez tagiem, ja teksta daļas nav", () => {
    const payload = {
      mimeType: "text/html",
      body: { data: encode("<p>Rēķins <b>€ 10</b></p>") },
    };
    assert.equal(extractPlainText(payload), "Rēķins € 10");
  });

  it("tukšu vēstuli atgriež kā tukšu virkni", () => {
    assert.equal(extractPlainText(undefined), "");
    assert.equal(extractPlainText({}), "");
  });
});
