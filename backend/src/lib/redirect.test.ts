import assert from "node:assert/strict";
import { describe, it } from "node:test";
import { safeReturnUrl, withStatus } from "./redirect.js";

describe("safeReturnUrl", () => {
  it("pieņem Expo Go adresi", () => {
    const url = "exp://192.168.1.10:8081/--/bank-callback";
    assert.equal(safeReturnUrl(url), url);
  });

  it("pieņem būvētas lietotnes shēmu", () => {
    assert.equal(
      safeReturnUrl("abonementi://bank-callback"),
      "abonementi://bank-callback",
    );
  });

  it("pieņem lokālu http adresi (Expo web)", () => {
    const url = "http://localhost:8081/bank-callback";
    assert.equal(safeReturnUrl(url), url);
  });

  it("pieņem lokālā tīkla adresi", () => {
    const url = "http://192.168.1.10:8081/bank-callback";
    assert.equal(safeReturnUrl(url), url);
  });

  it("noraida ārēju adresi — tas būtu atvērtais novirzītājs", () => {
    // Bez šī uzbrucējs varētu novirzīt lietotāju uz pīķšķerēšanas lapu
    // no mūsu pašu domēna.
    const result = safeReturnUrl("https://uzbrucejs.example.com/zagt");
    assert.notEqual(result, "https://uzbrucejs.example.com/zagt");
    assert.equal(result, "abonementi://bank-callback");
  });

  it("noraida javascript: shēmu", () => {
    const result = safeReturnUrl("javascript:alert(1)");
    assert.equal(result, "abonementi://bank-callback");
  });

  it("noraida data: shēmu", () => {
    assert.equal(
      safeReturnUrl("data:text/html,<script>alert(1)</script>"),
      "abonementi://bank-callback",
    );
  });

  it("noraida nederīgu adresi", () => {
    assert.equal(safeReturnUrl("nav adrese"), "abonementi://bank-callback");
  });

  it("bez adreses lieto noklusējumu", () => {
    assert.equal(safeReturnUrl(null), "abonementi://bank-callback");
    assert.equal(safeReturnUrl(undefined), "abonementi://bank-callback");
    assert.equal(safeReturnUrl(""), "abonementi://bank-callback");
  });
});

describe("withStatus", () => {
  it("pievieno parametrus adresei bez vaicājuma", () => {
    assert.equal(
      withStatus("abonementi://bank-callback", { status: "success" }),
      "abonementi://bank-callback?status=success",
    );
  });

  it("pievieno parametrus adresei, kurai jau ir vaicājums", () => {
    // Expo Go adresēs bieži jau ir parametri — & nevis ?
    assert.equal(
      withStatus("exp://10.0.0.5:8081/--/cb?a=1", { status: "error" }),
      "exp://10.0.0.5:8081/--/cb?a=1&status=error",
    );
  });

  it("kodē vērtības", () => {
    const result = withStatus("abonementi://cb", { reason: "bad request" });
    assert.ok(result.includes("reason=bad+request") || result.includes("reason=bad%20request"));
  });
});
