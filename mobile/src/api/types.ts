/** Backend atbilžu tipi. Atbilst backend serialize() funkcijām. */

export interface AuthUser {
  id: string;
  email: string;
  createdAt: string;
}

export interface AuthResponse {
  token: string;
  user: AuthUser;
}

export type SubscriptionCategory =
  | "streaming"
  | "utilities"
  | "insurance"
  | "telecom"
  | "subscription_software"
  | "food_delivery"
  | "gym_fitness"
  | "other";

export interface Subscription {
  id: string;
  merchantName: string;
  category: SubscriptionCategory | string | null;
  averageAmount: number | null;
  frequency: "monthly" | "weekly" | "yearly" | string | null;
  firstSeen: string | null;
  lastSeen: string | null;
  priceChangeDetected: boolean;
  priceChangePercent: number | null;
  isFlaggedUnwanted: boolean;
  occurrences: number | null;
  firstAmount: number | null;
  lastAmount: number | null;
  updatedAt: string;
}

export interface CategoryTotal {
  category: string;
  monthlyTotal: number;
  count: number;
}

export interface SubscriptionSummary {
  monthlyTotal: number;
  yearlyTotal: number;
  count: number;
  flaggedCount: number;
  priceChangeCount: number;
  byCategory: CategoryTotal[];
}

export interface SubscriptionListResponse {
  summary: SubscriptionSummary;
  subscriptions: Subscription[];
}

export interface TransactionHistoryItem {
  id: string;
  date: string;
  description: string;
  amount: number;
  currency: string;
}

export interface DraftAction {
  id: string;
  actionType: "cancel" | "negotiate" | string | null;
  draftText: string | null;
  status: string;
  createdAt: string;
}

export interface SubscriptionDetail extends Subscription {
  monthlyCost: number;
  drafts: DraftAction[];
  history: TransactionHistoryItem[];
}

export interface DraftResponse extends DraftAction {
  subscriptionId: string;
  merchantName: string;
}

export interface SyncResult {
  bankConnectionId: string;
  fetched: number;
  inserted: number;
  skipped: number;
}

export interface BankConnectResponse {
  bankConnectionId: string;
  authorizationUrl: string;
  state: string;
}

export interface BankConnection {
  id: string;
  provider: string | null;
  status: string;
  connectedAt: string;
  lastSyncedAt: string | null;
  providerAccountId: string | null;
}
