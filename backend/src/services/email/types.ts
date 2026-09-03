/** Viena vēstule, normalizēta neatkarīgi no pasta pakalpojuma. */
export interface EmailMessage {
  /** Pasta servisa ziņas ID — dedublikācijai. */
  externalId: string;
  receivedAt: Date;
  senderName: string;
  senderAddress: string;
  subject: string;
  /** Teksta saturs, apgriezts. Netiek glabāts datubāzē. */
  body: string;
}

export interface EmailAuthorizationStart {
  authorizationUrl: string;
  state: string;
}

export interface EmailAuthorizationResult {
  emailAddress: string;
  accessToken: string;
  refreshToken: string | null;
  expiresAt: Date | null;
}

export interface OutgoingEmail {
  to: string;
  subject: string;
  body: string;
}

/** Adapteris, ko implementē gan īstais Gmail klients, gan mock. */
export interface EmailProvider {
  readonly name: string;
  startAuthorization(state: string): Promise<EmailAuthorizationStart>;
  exchangeCallback(code: string): Promise<EmailAuthorizationResult>;
  /** Vēstules, kas varētu attiekties uz abonementiem vai būt krāpnieciskas. */
  fetchMessages(params: {
    accessToken: string;
    since: Date;
  }): Promise<EmailMessage[]>;
  sendMessage(params: {
    accessToken: string;
    from: string;
    email: OutgoingEmail;
  }): Promise<{ externalId: string }>;
}
