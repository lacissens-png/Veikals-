import assert from "node:assert/strict";
import { describe, it } from "node:test";
import { decryptToken, encryptToken, isEncrypted, secretsMatch } from "./crypto.js";
import { AppError } from "./errors.js";

/**
 * Testi paļaujas uz to, ka TOKEN_ENCRYPTION_KEY ir iestatīts vidē.
 * scripts/run-tests.sh to nodrošina.
 */
const SAMPLE = "eyJhbGciOiJIUzI1NiJ9.bankas-sesijas-tokens.signature";

describe("encryptToken / decryptToken", () => {
  it("apļa tests atgriež sākotnējo vērtību", () => {
    const encrypted = encryptToken(SAMPLE);
    assert.notEqual(encrypted, SAMPLE);
    assert.equal(decryptToken(encrypted), SAMPLE);
  });

  it("null paliek null — mock režīmā tokena nav", () => {
    assert.equal(encryptToken(null), null);
    assert.equal(decryptToken(null), null);
  });

  it("šifrētajā tekstā nav redzams sākotnējais saturs", () => {
    const encrypted = encryptToken(SAMPLE)!;
    assert.ok(!encrypted.includes("bankas-sesijas-tokens"));
  });

  it("viena un tā pati vērtība dod atšķirīgus šifrētos tekstus", () => {
    // Nejaušs IV: citādi vienādi tokeni būtu atpazīstami datubāzē.
    const a = encryptToken(SAMPLE);
    const b = encryptToken(SAMPLE);
    assert.notEqual(a, b);
    assert.equal(decryptToken(a), decryptToken(b));
  });

  it("formāts ir v1 ar četrām daļām", () => {
    const encrypted = encryptToken(SAMPLE)!;
    assert.ok(isEncrypted(encrypted));
    assert.equal(encrypted.split(":").length, 4);
  });

  it("tukšu virkni apstrādā korekti", () => {
    assert.equal(decryptToken(encryptToken("")), "");
  });

  it("garu tokenu apstrādā korekti", () => {
    const long = "x".repeat(4096);
    assert.equal(decryptToken(encryptToken(long)), long);
  });
});

describe("decryptToken — bojāti dati", () => {
  it("noraida pamainītu šifrēto tekstu", () => {
    // GCM autentifikācijas tags: viena baita maiņa padara ierakstu nederīgu.
    const encrypted = encryptToken(SAMPLE)!;
    const parts = encrypted.split(":");
    const data = Buffer.from(parts[3]!, "base64");
    data[0] = data[0]! ^ 0xff;
    parts[3] = data.toString("base64");

    assert.throws(() => decryptToken(parts.join(":")), AppError);
  });

  it("noraida pamainītu autentifikācijas tagu", () => {
    const encrypted = encryptToken(SAMPLE)!;
    const parts = encrypted.split(":");
    const tag = Buffer.from(parts[2]!, "base64");
    tag[0] = tag[0]! ^ 0xff;
    parts[2] = tag.toString("base64");

    assert.throws(() => decryptToken(parts.join(":")), AppError);
  });

  it("noraida nepilnīgu formātu", () => {
    assert.throws(() => decryptToken("v1:tikai:divas"), AppError);
  });
});

describe("decryptToken — mantotie ieraksti", () => {
  it("atklātā teksta ierakstu atgriež neskartu", () => {
    // Esošie savienojumi no laika pirms šifrēšanas nedrīkst salūzt.
    assert.equal(decryptToken("vecs-atklats-tokens"), "vecs-atklats-tokens");
  });

  it("atklātu tekstu neuzskata par šifrētu", () => {
    assert.ok(!isEncrypted("vecs-atklats-tokens"));
  });
});

describe("secretsMatch", () => {
  it("vienādas vērtības sakrīt", () => {
    assert.ok(secretsMatch("noslepums", "noslepums"));
  });

  it("atšķirīgas vērtības nesakrīt", () => {
    assert.ok(!secretsMatch("noslepums", "noslepumX"));
    assert.ok(!secretsMatch("iss", "daudz-garaks"));
  });
});
