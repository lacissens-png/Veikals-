import { randomBytes } from "node:crypto";
import { Prisma } from "../../generated/prisma/client.js";
import { env } from "../config/env.js";
import { decryptToken, encryptToken } from "../lib/crypto.js";
import { AppError } from "../lib/errors.js";
import { logger } from "../lib/logger.js";
import { prisma } from "../lib/prisma.js";
import { emailProvider } from "./email/gmail.service.js";
import { analyzeEmails } from "./emailAnalysis.service.js";

/** Sākuma datums vēstuļu lasīšanai. */
function syncFromDate(): Date {
  const date = new Date();
  date.setUTCMonth(date.getUTCMonth() - env.EMAIL_SYNC_MONTHS);
  date.setUTCHours(0, 0, 0, 0);
  return date;
}

export async function startConnection(userId: string) {
  const state = randomBytes(24).toString("hex");
  const { authorizationUrl } = await emailProvider.startAuthorization(state);

  const connection = await prisma.emailConnection.create({
    data: { userId, provider: emailProvider.name, status: "pending", authState: state },
    select: { id: true },
  });

  logger.info("E-pasta autorizācija sākta", { userId, emailConnectionId: connection.id });
  return { emailConnectionId: connection.id, authorizationUrl, state };
}

export async function completeConnection(code: string, state: string) {
  const connection = await prisma.emailConnection.findUnique({ where: { authState: state } });

  if (!connection) {
    throw AppError.badRequest("Nezināms vai jau izmantots `state` parametrs");
  }
  if (connection.status === "active") {
    throw AppError.conflict("Šis savienojums jau ir pabeigts");
  }

  try {
    const result = await emailProvider.exchangeCallback(code);

    return await prisma.emailConnection.update({
      where: { id: connection.id },
      data: {
        status: "active",
        emailAddress: result.emailAddress,
        accessToken: encryptToken(result.accessToken),
        refreshToken: encryptToken(result.refreshToken),
        tokenExpiresAt: result.expiresAt,
        connectedAt: new Date(),
        authState: null,
      },
      select: { id: true, userId: true, emailAddress: true },
    });
  } catch (error) {
    await prisma.emailConnection.update({
      where: { id: connection.id },
      data: { status: "error", authState: null },
    });
    throw error;
  }
}

export async function listConnections(userId: string) {
  return prisma.emailConnection.findMany({
    where: { userId },
    orderBy: { connectedAt: "desc" },
    select: {
      id: true,
      provider: true,
      emailAddress: true,
      status: true,
      connectedAt: true,
      lastSyncedAt: true,
    },
  });
}

export async function disconnect(userId: string, connectionId: string) {
  const connection = await prisma.emailConnection.findFirst({
    where: { id: connectionId, userId },
    select: { id: true },
  });
  if (!connection) throw AppError.notFound("E-pasta savienojums nav atrasts");

  await prisma.emailConnection.delete({ where: { id: connection.id } });
  logger.info("E-pasts atvienots", { userId, emailConnectionId: connectionId });
}

async function activeConnection(userId: string) {
  const connection = await prisma.emailConnection.findFirst({
    where: { userId, status: "active" },
    orderBy: { connectedAt: "desc" },
  });

  if (!connection) {
    throw AppError.badRequest(
      "Nav aktīva e-pasta savienojuma. Vispirms savieno pastu caur /api/email/connect.",
    );
  }
  return connection;
}

/**
 * Ielādē vēstules, palaiž AI analīzi un saglabā atradumus.
 * Vēstuļu saturs datubāzē NETIEK glabāts.
 */
export async function syncAndAnalyze(userId: string) {
  const connection = await activeConnection(userId);

  const messages = await emailProvider.fetchMessages({
    accessToken: decryptToken(connection.accessToken) ?? "",
    since: syncFromDate(),
  });

  // Jau apstrādātās vēstules netiek sūtītas uz API otrreiz.
  const known = await prisma.emailFinding.findMany({
    where: { emailConnectionId: connection.id },
    select: { externalId: true },
  });
  const seen = new Set(known.map((item) => item.externalId));
  const fresh = messages.filter((message) => !seen.has(message.externalId));

  const findings = await analyzeEmails(fresh);

  // Sasaistām ar jau atrastajiem abonementiem pēc tirgotāja nosaukuma.
  const subscriptions = await prisma.subscription.findMany({
    where: { userId },
    select: { id: true, merchantName: true },
  });

  const byMessage = new Map(fresh.map((m) => [m.externalId, m]));
  let stored = 0;

  for (const finding of findings) {
    const message = byMessage.get(finding.external_id);
    if (!message) continue;

    const match = subscriptions.find(
      (s) =>
        s.merchantName.toLowerCase().includes(finding.merchant_name.toLowerCase()) ||
        finding.merchant_name.toLowerCase().includes(s.merchantName.toLowerCase()),
    );

    await prisma.emailFinding.upsert({
      where: {
        finding_external_id: {
          emailConnectionId: connection.id,
          externalId: finding.external_id,
        },
      },
      create: {
        userId,
        emailConnectionId: connection.id,
        externalId: finding.external_id,
        receivedAt: message.receivedAt,
        kind: finding.kind,
        merchantName: finding.merchant_name,
        senderAddress: message.senderAddress,
        summary: finding.summary,
        amount:
          finding.amount === null ? null : new Prisma.Decimal(finding.amount.toFixed(2)),
        currency: finding.currency,
        effectiveDate: finding.effective_date
          ? new Date(`${finding.effective_date}T00:00:00.000Z`)
          : null,
        confidence: new Prisma.Decimal(finding.confidence.toFixed(2)),
        riskReasons: finding.risk_reasons.length > 0 ? finding.risk_reasons : undefined,
        subscriptionId: match?.id ?? null,
      },
      update: {},
    });
    stored += 1;
  }

  await prisma.emailConnection.update({
    where: { id: connection.id },
    data: { lastSyncedAt: new Date() },
  });

  logger.info("Vēstules sinhronizētas", {
    userId,
    fetched: messages.length,
    analyzed: fresh.length,
    stored,
  });

  return { fetched: messages.length, analyzed: fresh.length, stored };
}

type FindingRow = Awaited<ReturnType<typeof prisma.emailFinding.findFirstOrThrow>>;

function serialize(finding: FindingRow) {
  return {
    id: finding.id,
    kind: finding.kind,
    merchantName: finding.merchantName,
    senderAddress: finding.senderAddress,
    summary: finding.summary,
    amount: finding.amount ? Number(finding.amount) : null,
    currency: finding.currency,
    effectiveDate: finding.effectiveDate?.toISOString().slice(0, 10) ?? null,
    confidence: Number(finding.confidence),
    riskReasons: (finding.riskReasons as string[] | null) ?? [],
    status: finding.status,
    subscriptionId: finding.subscriptionId,
    receivedAt: finding.receivedAt.toISOString().slice(0, 10),
  };
}

/** Atradumi lietotnei: brīdinājumi atsevišķi no gaidāmajiem maksājumiem. */
export async function listFindings(userId: string) {
  const findings = await prisma.emailFinding.findMany({
    where: { userId, status: { not: "dismissed" } },
    orderBy: [{ receivedAt: "desc" }],
  });

  const all = findings.map(serialize);

  return {
    summary: {
      total: all.length,
      suspicious: all.filter((f) => f.kind === "suspicious").length,
      upcoming: all.filter((f) => f.kind !== "suspicious" && f.kind !== "invoice").length,
      newCount: all.filter((f) => f.status === "new").length,
    },
    suspicious: all.filter((f) => f.kind === "suspicious"),
    upcoming: all.filter((f) => f.kind !== "suspicious"),
  };
}

export async function setFindingStatus(
  userId: string,
  id: string,
  status: "acknowledged" | "dismissed",
) {
  const finding = await prisma.emailFinding.findFirst({
    where: { id, userId },
    select: { id: true },
  });
  if (!finding) throw AppError.notFound("Atradums nav atrasts");

  return serialize(
    await prisma.emailFinding.update({ where: { id }, data: { status } }),
  );
}

/**
 * Nosūta sagatavoto melnrakstu no lietotāja pasta.
 *
 * Adresāts nāk no paša tirgotāja vēstulēm pastkastītē — tāpēc e-pasta
 * savienojums ir priekšnosacījums, nevis tikai ērtība.
 */
export async function sendDraft(userId: string, draftId: string) {
  const draft = await prisma.draftAction.findFirst({
    where: { id: draftId, subscription: { userId } },
    include: { subscription: { select: { id: true, merchantName: true } } },
  });

  if (!draft) throw AppError.notFound("Melnraksts nav atrasts");
  if (draft.sentAt) throw AppError.conflict("Šis melnraksts jau ir nosūtīts");
  if (!draft.draftText) throw AppError.badRequest("Melnrakstam nav teksta");

  const connection = await activeConnection(userId);

  const contact = await prisma.emailFinding.findFirst({
    where: {
      userId,
      senderAddress: { not: null },
      merchantName: { contains: draft.subscription.merchantName.split(" ")[0] ?? "", mode: "insensitive" },
    },
    orderBy: { receivedAt: "desc" },
    select: { senderAddress: true },
  });

  if (!contact?.senderAddress) {
    throw AppError.badRequest(
      `Nav atrasta ${draft.subscription.merchantName} kontaktadrese pastkastītē. ` +
        "Nokopē tekstu un nosūti pats.",
    );
  }

  const [subject, ...rest] = draft.draftText.split("\n");

  const sent = await emailProvider.sendMessage({
    accessToken: decryptToken(connection.accessToken) ?? "",
    from: connection.emailAddress ?? "",
    email: {
      to: contact.senderAddress,
      subject: (subject ?? "").replace(/^(Subject|Onderwerp|Temats):\s*/i, "").trim() ||
        `Opzegging ${draft.subscription.merchantName}`,
      body: rest.join("\n").trim() || draft.draftText,
    },
  });

  const updated = await prisma.draftAction.update({
    where: { id: draft.id },
    data: { sentAt: new Date(), recipient: contact.senderAddress, status: "sent" },
  });

  logger.info("Melnraksts nosūtīts", { userId, draftId, mock: env.GMAIL_MOCK });

  return {
    id: updated.id,
    recipient: updated.recipient,
    sentAt: updated.sentAt,
    externalId: sent.externalId,
    /** Mock režīmā vēstule NETIEK sūtīta — lietotnei tas jāparāda. */
    simulated: env.GMAIL_MOCK,
  };
}
