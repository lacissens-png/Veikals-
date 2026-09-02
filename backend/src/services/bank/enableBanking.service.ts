import { randomUUID } from "node:crypto";
import { env } from "../../config/env.js";
import { AppError } from "../../lib/errors.js";
import { logger } from "../../lib/logger.js";
import { generateMockTransactions } from "./fixtures.js";
import { normalizeAll } from "./normalize.js";
import type { RawTransaction } from "./normalize.js";
import type {
  AuthorizationResult,
  AuthorizationStart,
  BankProvider,
} from "./types.js";

/**
 * Enable Banking (PSD2) adapteris.
 *
 * SVARĪGI PIRMS PRODUKCIJAS: Enable Banking īstā autorizācija paraksta katru
 * pieprasījumu ar RS256 JWT, kas veidots no aplikācijas ID un privātās atslēgas,
 * nevis ar vienkāršu client_id/client_secret pāri (kā norādīts MVP
 * specifikācijā). Zemāk esošā `buildAuthHeader` ir vienīgā vieta, kas jāmaina,
 * kad tiek saņemtas īstās atslēgas — skat. README sadaļu "Enable Banking".
 *
 * Endpointi atbilst specifikācijai:
 *   POST /auth                          — autorizācijas sākums
 *   POST /sessions                      — koda apmaiņa pret sesiju
 *   GET  /accounts/{id}/transactions    — darījumu vēsture
 */

function buildAuthHeader(): Record<string, string> {
  if (!env.ENABLE_BANKING_CLIENT_ID || !env.ENABLE_BANKING_CLIENT_SECRET) {
    throw AppError.unavailable(
      "Enable Banking atslēgas nav konfigurētas. Iestati ENABLE_BANKING_CLIENT_ID " +
        "un ENABLE_BANKING_CLIENT_SECRET, vai lieto ENABLE_BANKING_MOCK=true.",
    );
  }

  const basic = Buffer.from(
    `${env.ENABLE_BANKING_CLIENT_ID}:${env.ENABLE_BANKING_CLIENT_SECRET}`,
  ).toString("base64");

  return {
    Authorization: `Basic ${basic}`,
    "Content-Type": "application/json",
  };
}

async function request<T>(
  path: string,
  init: { method: string; body?: unknown } = { method: "GET" },
): Promise<T> {
  const url = `${env.ENABLE_BANKING_API_URL}${path}`;

  let response: Response;
  try {
    response = await fetch(url, {
      method: init.method,
      headers: buildAuthHeader(),
      body: init.body === undefined ? undefined : JSON.stringify(init.body),
    });
  } catch (error) {
    logger.error("Enable Banking savienojuma kļūda", {
      path,
      message: error instanceof Error ? error.message : String(error),
    });
    throw AppError.upstream("Neizdevās sasniegt bankas API");
  }

  const text = await response.text();
  const payload: unknown = text ? safeJsonParse(text) : null;

  if (!response.ok) {
    logger.error("Enable Banking atgrieza kļūdu", {
      path,
      status: response.status,
      payload,
    });
    throw AppError.upstream(
      `Bankas API kļūda (${response.status})`,
      env.NODE_ENV === "production" ? undefined : payload,
    );
  }

  return payload as T;
}

function safeJsonParse(text: string): unknown {
  try {
    return JSON.parse(text);
  } catch {
    return { raw: text };
  }
}

/** Enable Banking summas nāk kā {amount: "12.34", currency: "EUR"}. */
const httpProvider: BankProvider = {
  name: "enable_banking",

  async startAuthorization(state) {
    const payload = await request<{ url?: string; authorization_url?: string }>(
      "/auth",
      {
        method: "POST",
        body: {
          access: { valid_until: validUntil() },
          redirect_url: env.ENABLE_BANKING_REDIRECT_URI,
          state,
        },
      },
    );

    const authorizationUrl = payload.url ?? payload.authorization_url;
    if (!authorizationUrl) {
      throw AppError.upstream("Bankas API neatgrieza autorizācijas saiti");
    }

    return { authorizationUrl, state };
  },

  async exchangeCallback(code) {
    const session = await request<{
      session_id?: string;
      access_token?: string;
      expires_at?: string;
      accounts?: Array<{ uid?: string; account_id?: { iban?: string } }>;
    }>("/sessions", { method: "POST", body: { code } });

    const accountId = session.accounts?.[0]?.uid;
    if (!accountId) {
      throw AppError.upstream("Bankas sesijā nav neviena konta");
    }

    return {
      sessionId: session.session_id ?? code,
      accessToken: session.access_token ?? null,
      expiresAt: session.expires_at ? new Date(session.expires_at) : null,
      accountId,
    };
  },

  async fetchTransactions({ accountId, from }) {
    const query = new URLSearchParams({
      date_from: from.toISOString().slice(0, 10),
    });

    const payload = await request<{ transactions?: RawTransaction[] }>(
      `/accounts/${encodeURIComponent(accountId)}/transactions?${query.toString()}`,
    );

    return normalizeAll(payload.transactions ?? []);
  },
};

/** Mock režīms — visa plūsma bez īsta bankas savienojuma. */
const mockProvider: BankProvider = {
  name: "enable_banking_mock",

  async startAuthorization(state) {
    // Norāda uz mūsu pašu callback, lai "banka" uzreiz atgriež lietotāju atpakaļ.
    const url = new URL(env.ENABLE_BANKING_REDIRECT_URI);
    url.searchParams.set("code", `mock-code-${randomUUID()}`);
    url.searchParams.set("state", state);
    return { authorizationUrl: url.toString(), state };
  },

  async exchangeCallback(code) {
    return {
      sessionId: `mock-session-${code.slice(-8)}`,
      accessToken: null,
      expiresAt: new Date(Date.now() + 90 * 24 * 60 * 60 * 1000),
      accountId: "mock-account-NL91ABNA0417164300",
    };
  },

  async fetchTransactions() {
    return generateMockTransactions(env.TRANSACTION_SYNC_MONTHS);
  },
};

function validUntil(): string {
  const date = new Date();
  date.setUTCDate(date.getUTCDate() + 90);
  return date.toISOString();
}

/** Aktīvais datu sniedzējs — atkarīgs no ENABLE_BANKING_MOCK. */
export const bankProvider: BankProvider = env.ENABLE_BANKING_MOCK
  ? mockProvider
  : httpProvider;
