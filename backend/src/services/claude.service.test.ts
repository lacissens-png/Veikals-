import assert from "node:assert/strict";
import { describe, it } from "node:test";
import Anthropic from "@anthropic-ai/sdk";
import { AppError } from "../lib/errors.js";
import {
  analysisSchema,
  analyzeTransactions,
  buildAnalysisRequest,
  buildDraftRequest,
  CATEGORIES,
  toAppError,
  type TransactionForAnalysis,
} from "./claude.service.js";

const transactions: TransactionForAnalysis[] = [
  { date: "2026-08-26", description: "Ziggo Services B.V.", amount: -37.22 },
  { date: "2026-07-27", description: "Ziggo Services B.V.", amount: -37.22 },
];

describe("buildAnalysisRequest — privātums", () => {
  it("sūta tikai datumu, aprakstu un summu", () => {
    const request = buildAnalysisRequest(transactions);
    const content = request.messages[0]!.content as string;

    // Izvelkam JSON masīvu no prompta un pārbaudām katra objekta atslēgas.
    const json = content.slice(content.indexOf("["), content.lastIndexOf("]") + 1);
    const sent = JSON.parse(json) as Array<Record<string, unknown>>;

    assert.equal(sent.length, 2);
    for (const item of sent) {
      assert.deepEqual(Object.keys(item).sort(), [
        "amount",
        "date",
        "description",
      ]);
    }
  });

  it("nogriež laukus, ko izsaucējs pievienojis papildus", () => {
    // Aizsardzība pret nākotnes izmaiņu, kas nejauši ievilktu personas datus.
    const withExtras = [
      {
        ...transactions[0]!,
        userId: "e4feb4eb-83fc-4935-935b-573934cf0aff",
        email: "lietotajs@example.com",
        iban: "NL91ABNA0417164300",
      },
    ] as unknown as TransactionForAnalysis[];

    const content = buildAnalysisRequest(withExtras).messages[0]!
      .content as string;
    const serialized = JSON.stringify(buildAnalysisRequest(withExtras));

    for (const secret of [
      "lietotajs@example.com",
      "NL91ABNA0417164300",
      "e4feb4eb-83fc-4935-935b-573934cf0aff",
      "userId",
      "email",
      "iban",
    ]) {
      assert.ok(
        !content.includes(secret),
        `promptā nedrīkst būt "${secret}"`,
      );
      assert.ok(
        !serialized.includes(secret),
        `pieprasījumā nedrīkst būt "${secret}"`,
      );
    }
  });

  it("izmanto konfigurēto modeli un adaptīvo domāšanu", () => {
    const request = buildAnalysisRequest(transactions);
    assert.equal(request.model, "claude-opus-5");
    assert.deepEqual(request.thinking, { type: "adaptive" });
    assert.ok(request.output_config.format, "jābūt strukturētai izvadei");
    assert.ok(request.max_tokens > 0);
  });

  it("sistēmas promptā ir uzskaitītas visas kategorijas", () => {
    const system = buildAnalysisRequest(transactions).system;
    for (const category of CATEGORIES) {
      assert.ok(system.includes(category), `promptā trūkst "${category}"`);
    }
  });
});

describe("analyzeTransactions", () => {
  it("tukšam sarakstam neizsauc API un neprasa atslēgu", async () => {
    // Svarīgi: bez šī pārbaudījuma tukša sinhronizācija mestu 503.
    assert.deepEqual(await analyzeTransactions([]), []);
  });
});

describe("buildDraftRequest", () => {
  it("atcelšanas promptā ir tirgotāja nosaukums un lūgums apstiprināt", () => {
    const content = buildDraftRequest("Ziggo Services B.V.", "cancel")
      .messages[0]!.content as string;

    assert.ok(content.includes("Ziggo Services B.V."));
    assert.ok(content.includes("atcelt"));
    assert.ok(content.includes("apstiprinājumu"));
  });

  it("pārrunu prompts atšķiras no atcelšanas prompta", () => {
    const cancel = buildDraftRequest("Ziggo", "cancel").messages[0]!.content;
    const negotiate = buildDraftRequest("Ziggo", "negotiate").messages[0]!
      .content;

    assert.notEqual(cancel, negotiate);
    assert.ok((negotiate as string).includes("konkurenta"));
  });

  it("melnrakstiem izmanto zemu piepūli — tas ir īss uzdevums", () => {
    const request = buildDraftRequest("Ziggo", "cancel");
    assert.equal(request.output_config.effort, "low");
    assert.equal(request.model, "claude-opus-5");
  });
});

describe("analysisSchema", () => {
  const valid = {
    merchant_name: "Ziggo Services B.V.",
    category: "telecom",
    average_amount: 37.22,
    frequency: "monthly",
    occurrences: 3,
    first_date: "2026-06-26",
    last_date: "2026-08-26",
    price_changed: false,
    price_change_percent: 0,
    first_amount: 37.22,
    last_amount: 37.22,
  };

  it("pieņem derīgu atbildi", () => {
    const result = analysisSchema.safeParse({ subscriptions: [valid] });
    assert.ok(result.success);
  });

  it("pieņem tukšu sarakstu", () => {
    assert.ok(analysisSchema.safeParse({ subscriptions: [] }).success);
  });

  it("noraida izdomātu kategoriju", () => {
    // Bez šī modelis varētu ieviest kategoriju, ko frontend nepazīst.
    const result = analysisSchema.safeParse({
      subscriptions: [{ ...valid, category: "kaut_kas_jauns" }],
    });
    assert.ok(!result.success);
  });

  it("noraida nederīgu biežumu", () => {
    const result = analysisSchema.safeParse({
      subscriptions: [{ ...valid, frequency: "reizēm" }],
    });
    assert.ok(!result.success);
  });

  it("noraida datumu nepareizā formātā", () => {
    const result = analysisSchema.safeParse({
      subscriptions: [{ ...valid, first_date: "26.06.2026" }],
    });
    assert.ok(!result.success);
  });

  it("noraida ierakstu ar trūkstošu lauku", () => {
    const { average_amount, ...incomplete } = valid;
    const result = analysisSchema.safeParse({ subscriptions: [incomplete] });
    assert.ok(!result.success);
  });
});

describe("toAppError", () => {
  function statusOf(error: unknown): number {
    const mapped = toAppError(error, "tests");
    assert.ok(mapped instanceof AppError);
    return mapped.status;
  }

  it("ātruma ierobežojums kļūst par 429, nevis 500", () => {
    const error = new Anthropic.RateLimitError(429, undefined, "slow down", new Headers());
    const mapped = toAppError(error, "tests");
    assert.equal(mapped.status, 429);
    assert.equal(mapped.code, "rate_limited");
  });

  it("nederīga atslēga kļūst par 503", () => {
    const error = new Anthropic.AuthenticationError(401, undefined, "bad key", new Headers());
    assert.equal(statusOf(error), 503);
  });

  it("nezināms modelis kļūst par 502", () => {
    const error = new Anthropic.NotFoundError(404, undefined, "no model", new Headers());
    assert.equal(statusOf(error), 502);
  });

  it("negaidīta kļūda kļūst par 502, nevis netiek izmesta tālāk", () => {
    assert.equal(statusOf(new Error("kaut kas cits")), 502);
  });

  it("jau esošu AppError atstāj neskartu", () => {
    const original = AppError.badRequest("nederīgi dati");
    assert.equal(toAppError(original, "tests"), original);
  });

  it("nekad neatklāj iekšējo kļūdas tekstu lietotājam", () => {
    const mapped = toAppError(new Error("SECRET_TOKEN=abc123"), "tests");
    assert.ok(!mapped.message.includes("SECRET_TOKEN"));
  });
});
