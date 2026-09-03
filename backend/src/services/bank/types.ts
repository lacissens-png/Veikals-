/** Viens bankas darījums, normalizēts neatkarīgi no bankas datu sniedzēja. */
export interface BankTransaction {
  /** Bankas puses ID, ja tāds ir — izmanto dedublikācijai atkārtotā sinhronizācijā. */
  externalId: string | null;
  /** ISO datums YYYY-MM-DD. */
  date: string;
  description: string;
  /** Negatīvs = izdevums, pozitīvs = ieskaitījums. */
  amount: number;
  currency: string;
  /** Neapstrādātā bankas atbilde — saglabājas transactions.raw_data. */
  raw: unknown;
}

/** Autorizācijas sākums: kur sūtīt lietotāju un ar kādu `state`. */
export interface AuthorizationStart {
  authorizationUrl: string;
  state: string;
}

/** Rezultāts pēc tam, kad lietotājs atgriezies no bankas. */
export interface AuthorizationResult {
  sessionId: string;
  accessToken: string | null;
  expiresAt: Date | null;
  /** Pirmā konta ID — MVP strādā ar vienu kontu. */
  accountId: string;
}

/** Adapteris, ko implementē gan īstais Enable Banking klients, gan mock. */
export interface BankProvider {
  readonly name: string;
  startAuthorization(state: string): Promise<AuthorizationStart>;
  exchangeCallback(code: string): Promise<AuthorizationResult>;
  fetchTransactions(params: {
    accountId: string;
    accessToken: string | null;
    from: Date;
  }): Promise<BankTransaction[]>;
}
