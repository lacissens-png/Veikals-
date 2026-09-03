import { Platform } from "react-native";
import type {
  AuthResponse,
  EmailConnection,
  FindingsResponse,
  SendDraftResult,
  BankConnectResponse,
  BankConnection,
  DraftResponse,
  Subscription,
  SubscriptionDetail,
  SubscriptionListResponse,
  SyncResult,
} from "./types";

/**
 * Backend adrese. Uz fiziskas ierīces `localhost` norāda uz pašu ierīci,
 * tāpēc izstrādē jānorāda datora IP caur EXPO_PUBLIC_API_URL, piem.:
 *   EXPO_PUBLIC_API_URL=http://192.168.1.10:4000 npx expo start
 */
export const API_BASE_URL =
  process.env.EXPO_PUBLIC_API_URL ??
  (Platform.OS === "android"
    ? "http://10.0.2.2:4000" // Android emulatora ceļš uz resursdatora localhost
    : "http://localhost:4000");

/** Kļūda ar backend atgriezto kodu un lietotājam paredzēto ziņojumu. */
export class ApiError extends Error {
  readonly status: number;
  readonly code: string;

  constructor(status: number, code: string, message: string) {
    super(message);
    this.name = "ApiError";
    this.status = status;
    this.code = code;
  }

  /** true, ja lietotājs jāizmet uz sākuma ekrānu. */
  get isUnauthorized(): boolean {
    return this.status === 401;
  }
}

interface BackendErrorShape {
  error?: { code?: string; message?: string };
}

let authToken: string | null = null;

export function setAuthToken(token: string | null): void {
  authToken = token;
}

async function request<T>(
  path: string,
  options: { method?: string; body?: unknown } = {},
): Promise<T> {
  const headers: Record<string, string> = { Accept: "application/json" };

  if (options.body !== undefined) {
    headers["Content-Type"] = "application/json";
  }
  if (authToken) {
    headers.Authorization = `Bearer ${authToken}`;
  }

  let response: Response;
  try {
    response = await fetch(`${API_BASE_URL}${path}`, {
      method: options.method ?? "GET",
      headers,
      body: options.body === undefined ? undefined : JSON.stringify(options.body),
    });
  } catch {
    throw new ApiError(
      0,
      "network_error",
      `Nevar sasniegt serveri (${API_BASE_URL}). Pārbaudi, vai backend darbojas ` +
        "un vai EXPO_PUBLIC_API_URL norāda uz pareizo adresi.",
    );
  }

  if (response.status === 204) {
    return undefined as T;
  }

  const text = await response.text();
  let payload: unknown = null;

  if (text) {
    try {
      payload = JSON.parse(text);
    } catch {
      payload = null;
    }
  }

  if (!response.ok) {
    const shape = payload as BackendErrorShape | null;
    throw new ApiError(
      response.status,
      shape?.error?.code ?? "unknown_error",
      shape?.error?.message ?? `Servera kļūda (${response.status})`,
    );
  }

  return payload as T;
}

export const api = {
  register: (email: string, password: string) =>
    request<AuthResponse>("/api/auth/register", {
      method: "POST",
      body: { email, password },
    }),

  login: (email: string, password: string) =>
    request<AuthResponse>("/api/auth/login", {
      method: "POST",
      body: { email, password },
    }),

  connectBank: () =>
    request<BankConnectResponse>("/api/bank/connect", { method: "POST" }),

  listConnections: () =>
    request<{ connections: BankConnection[] }>("/api/bank/connections"),

  disconnectBank: (id: string) =>
    request<void>(`/api/bank/connections/${id}`, { method: "DELETE" }),

  syncTransactions: () =>
    request<SyncResult>("/api/transactions/sync", { method: "POST" }),

  analyze: () =>
    request<SubscriptionListResponse>("/api/subscriptions/analyze", {
      method: "POST",
    }),

  listSubscriptions: () =>
    request<SubscriptionListResponse>("/api/subscriptions"),

  getSubscription: (id: string) =>
    request<SubscriptionDetail>(`/api/subscriptions/${id}`),

  setUnwanted: (id: string, isFlaggedUnwanted: boolean) =>
    request<Subscription>(`/api/subscriptions/${id}`, {
      method: "PATCH",
      body: { isFlaggedUnwanted },
    }),

  createDraft: (id: string, actionType: "cancel" | "negotiate") =>
    request<DraftResponse>(
      `/api/subscriptions/${id}/${actionType === "cancel" ? "draft-cancel" : "draft-negotiate"}`,
      { method: "POST" },
    ),

  markDraft: (draftId: string, status: "copied" | "dismissed") =>
    request<{ id: string; status: string }>(
      `/api/subscriptions/drafts/${draftId}`,
      { method: "PATCH", body: { status } },
    ),

  registerPushToken: (pushToken: string) =>
    request<void>("/api/notifications/token", {
      method: "POST",
      body: { pushToken },
    }),

  connectEmail: () =>
    request<{ emailConnectionId: string; authorizationUrl: string }>(
      "/api/email/connect",
      { method: "POST" },
    ),

  listEmailConnections: () =>
    request<{ connections: EmailConnection[] }>("/api/email/connections"),

  disconnectEmail: (id: string) =>
    request<void>(`/api/email/connections/${id}`, { method: "DELETE" }),

  syncEmail: () =>
    request<{ fetched: number; analyzed: number; stored: number }>(
      "/api/email/sync",
      { method: "POST" },
    ),

  listFindings: () => request<FindingsResponse>("/api/email/findings"),

  setFindingStatus: (id: string, status: "acknowledged" | "dismissed") =>
    request<unknown>(`/api/email/findings/${id}`, {
      method: "PATCH",
      body: { status },
    }),

  sendDraft: (draftId: string) =>
    request<SendDraftResult>(`/api/subscriptions/drafts/${draftId}/send`, {
      method: "POST",
    }),

  deleteAccount: () => request<void>("/api/me", { method: "DELETE" }),
};
