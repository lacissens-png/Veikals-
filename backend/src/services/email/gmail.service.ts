import { randomUUID } from "node:crypto";
import { env } from "../../config/env.js";
import { AppError } from "../../lib/errors.js";
import { logger } from "../../lib/logger.js";
import { generateMockEmails } from "./fixtures.js";
import type {
  EmailAuthorizationResult,
  EmailAuthorizationStart,
  EmailMessage,
  EmailProvider,
  OutgoingEmail,
} from "./types.js";

/**
 * Gmail adapteris.
 *
 * Atļaujas (scopes), kas nepieciešamas:
 *   gmail.readonly — lasīt vēstules, lai atrastu abonementus un krāpšanu
 *   gmail.send     — nosūtīt atcelšanas vēstuli lietotāja vārdā
 *
 * Apzināti NETIEK prasīts gmail.modify: lietotne brīdina par krāpšanu, bet
 * nekad neaiztiek pastkastīti. Nepareizi aizmests īsts rēķins nozīmētu
 * nokavētu maksājumu, un tas ir sliktāk nekā brīdinājums, ko lietotājs redz.
 */

const SCOPES = [
  "https://www.googleapis.com/auth/gmail.readonly",
  "https://www.googleapis.com/auth/gmail.send",
];

/**
 * Meklēšanas vaicājums. Ievērojami sašaurina to, kas vispār tiek lasīts —
 * nevis visa pastkastīte, bet vēstules, kas var attiekties uz abonementiem
 * vai izskatīties pēc krāpšanas. Divvalodīgs: NL un EN.
 */
export function buildSearchQuery(since: Date): string {
  const after = since.toISOString().slice(0, 10).replace(/-/g, "/");
  const terms = [
    "abonnement", "subscription", "proefperiode", "trial",
    "factuur", "invoice", "verlenging", "renewal",
    "opzeggen", "cancel", "betaling", "payment",
    "prijswijziging", "price change",
  ];
  return `after:${after} (${terms.map((t) => `"${t}"`).join(" OR ")})`;
}

function requireCredentials(): { clientId: string; clientSecret: string } {
  if (!env.GMAIL_CLIENT_ID || !env.GMAIL_CLIENT_SECRET) {
    throw AppError.unavailable(
      "Gmail atslēgas nav konfigurētas. Iestati GMAIL_CLIENT_ID un " +
        "GMAIL_CLIENT_SECRET, vai lieto GMAIL_MOCK=true.",
    );
  }
  return {
    clientId: env.GMAIL_CLIENT_ID,
    clientSecret: env.GMAIL_CLIENT_SECRET,
  };
}

interface GmailHeader {
  name?: string;
  value?: string;
}

interface GmailPayload {
  headers?: GmailHeader[];
  mimeType?: string;
  body?: { data?: string };
  parts?: GmailPayload[];
}

function header(payload: GmailPayload | undefined, name: string): string {
  const found = payload?.headers?.find(
    (h) => h.name?.toLowerCase() === name.toLowerCase(),
  );
  return found?.value ?? "";
}

/** Gmail base64url -> teksts. */
function decodeBody(data: string | undefined): string {
  if (!data) return "";
  return Buffer.from(data.replace(/-/g, "+").replace(/_/g, "/"), "base64").toString(
    "utf8",
  );
}

/** Rekursīvi meklē konkrēta tipa daļu. */
function findPart(payload: GmailPayload | undefined, mimeType: string): string {
  if (!payload) return "";

  if (payload.mimeType === mimeType && payload.body?.data) {
    return decodeBody(payload.body.data);
  }

  for (const part of payload.parts ?? []) {
    const found = findPart(part, mimeType);
    if (found) return found;
  }

  return "";
}

/**
 * Izvelk vēstules tekstu.
 *
 * Vispirms tiek pārmeklēts VISS koks pēc text/plain, un tikai tad, ja tāda nav
 * nekur, tiek ņemts HTML. Meklējot abus vienlaikus, multipart vēstulē uzvarētu
 * tā daļa, kas gadās pirmā — parasti HTML.
 */
export function extractPlainText(payload: GmailPayload | undefined): string {
  const plain = findPart(payload, "text/plain");
  if (plain) return plain;

  const html = findPart(payload, "text/html");
  if (!html) return "";

  return html
    .replace(/<[^>]+>/g, " ")
    .replace(/\s+/g, " ")
    .trim();
}

/** Adrese formā "Vārds <adrese@domēns>" -> abas daļas atsevišķi. */
export function parseSender(raw: string): { name: string; address: string } {
  const match = raw.match(/^\s*"?([^"<]*?)"?\s*<([^>]+)>\s*$/);
  if (match) {
    return { name: match[1]!.trim(), address: match[2]!.trim().toLowerCase() };
  }
  const address = raw.trim().toLowerCase();
  return { name: address, address };
}

/** Ķermenis tiek apgriezts — modelim pietiek, un mazāk datu izceļo. */
export const MAX_BODY_CHARS = 2000;

const httpProvider: EmailProvider = {
  name: "gmail",

  async startAuthorization(state) {
    const { clientId } = requireCredentials();
    const url = new URL("https://accounts.google.com/o/oauth2/v2/auth");
    url.searchParams.set("client_id", clientId);
    url.searchParams.set("redirect_uri", env.GMAIL_REDIRECT_URI);
    url.searchParams.set("response_type", "code");
    url.searchParams.set("scope", SCOPES.join(" "));
    url.searchParams.set("access_type", "offline");
    url.searchParams.set("prompt", "consent");
    url.searchParams.set("state", state);
    return { authorizationUrl: url.toString(), state };
  },

  async exchangeCallback(code) {
    const { clientId, clientSecret } = requireCredentials();

    const response = await fetch("https://oauth2.googleapis.com/token", {
      method: "POST",
      headers: { "Content-Type": "application/x-www-form-urlencoded" },
      body: new URLSearchParams({
        code,
        client_id: clientId,
        client_secret: clientSecret,
        redirect_uri: env.GMAIL_REDIRECT_URI,
        grant_type: "authorization_code",
      }),
    });

    if (!response.ok) {
      logger.error("Gmail token apmaiņa neizdevās", { status: response.status });
      throw AppError.upstream("Neizdevās pabeigt Gmail autorizāciju");
    }

    const token = (await response.json()) as {
      access_token?: string;
      refresh_token?: string;
      expires_in?: number;
    };

    if (!token.access_token) {
      throw AppError.upstream("Gmail neatgrieza piekļuves tokenu");
    }

    const profile = await fetch(
      "https://gmail.googleapis.com/gmail/v1/users/me/profile",
      { headers: { Authorization: `Bearer ${token.access_token}` } },
    );
    const email = profile.ok
      ? ((await profile.json()) as { emailAddress?: string }).emailAddress
      : undefined;

    return {
      emailAddress: email ?? "nezināms",
      accessToken: token.access_token,
      refreshToken: token.refresh_token ?? null,
      expiresAt: token.expires_in
        ? new Date(Date.now() + token.expires_in * 1000)
        : null,
    };
  },

  async fetchMessages({ accessToken, since }) {
    const list = await fetch(
      "https://gmail.googleapis.com/gmail/v1/users/me/messages?" +
        new URLSearchParams({ q: buildSearchQuery(since), maxResults: "50" }),
      { headers: { Authorization: `Bearer ${accessToken}` } },
    );

    if (!list.ok) {
      throw AppError.upstream(`Gmail API kļūda (${list.status})`);
    }

    const { messages = [] } = (await list.json()) as {
      messages?: Array<{ id: string }>;
    };

    const results: EmailMessage[] = [];

    for (const { id } of messages) {
      const detail = await fetch(
        `https://gmail.googleapis.com/gmail/v1/users/me/messages/${id}?format=full`,
        { headers: { Authorization: `Bearer ${accessToken}` } },
      );
      if (!detail.ok) continue;

      const message = (await detail.json()) as {
        id: string;
        internalDate?: string;
        payload?: GmailPayload;
      };

      const sender = parseSender(header(message.payload, "From"));
      results.push({
        externalId: message.id,
        receivedAt: new Date(Number(message.internalDate ?? Date.now())),
        senderName: sender.name,
        senderAddress: sender.address,
        subject: header(message.payload, "Subject"),
        body: extractPlainText(message.payload).slice(0, MAX_BODY_CHARS),
      });
    }

    return results;
  },

  async sendMessage({ accessToken, from, email }) {
    // RFC 2822 ziņa, base64url kodēta, kā prasa Gmail API.
    const raw = Buffer.from(
      [
        `From: ${from}`,
        `To: ${email.to}`,
        `Subject: ${email.subject}`,
        "Content-Type: text/plain; charset=UTF-8",
        "",
        email.body,
      ].join("\r\n"),
    )
      .toString("base64")
      .replace(/\+/g, "-")
      .replace(/\//g, "_")
      .replace(/=+$/, "");

    const response = await fetch(
      "https://gmail.googleapis.com/gmail/v1/users/me/messages/send",
      {
        method: "POST",
        headers: {
          Authorization: `Bearer ${accessToken}`,
          "Content-Type": "application/json",
        },
        body: JSON.stringify({ raw }),
      },
    );

    if (!response.ok) {
      logger.error("Gmail sūtīšana neizdevās", { status: response.status });
      throw AppError.upstream("Neizdevās nosūtīt vēstuli");
    }

    const sent = (await response.json()) as { id?: string };
    return { externalId: sent.id ?? "nezināms" };
  },
};

/** Mock režīms — visa plūsma bez Google Cloud projekta. */
const mockProvider: EmailProvider = {
  name: "gmail_mock",

  async startAuthorization(state) {
    const url = new URL(env.GMAIL_REDIRECT_URI);
    url.searchParams.set("code", `mock-gmail-${randomUUID()}`);
    url.searchParams.set("state", state);
    return { authorizationUrl: url.toString(), state };
  },

  async exchangeCallback() {
    return {
      emailAddress: "demo@gmail.com",
      accessToken: "mock-gmail-access-token",
      refreshToken: "mock-gmail-refresh-token",
      expiresAt: new Date(Date.now() + 3600 * 1000),
    };
  },

  async fetchMessages() {
    return generateMockEmails();
  },

  async sendMessage({ email }) {
    logger.info("Mock režīms: vēstule NETIEK sūtīta", {
      to: email.to,
      subject: email.subject,
    });
    return { externalId: `mock-sent-${randomUUID()}` };
  },
};

export const emailProvider: EmailProvider = env.GMAIL_MOCK
  ? mockProvider
  : httpProvider;
