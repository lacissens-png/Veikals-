/**
 * Aplikācijas kļūda ar HTTP statusu. Visas paredzamās kļūdas met šo klasi,
 * lai `errorHandler` varētu atgriezt konsekventu JSON atbildi.
 */
export class AppError extends Error {
  readonly status: number;
  readonly code: string;
  readonly details?: unknown;

  constructor(
    status: number,
    code: string,
    message: string,
    details?: unknown,
  ) {
    super(message);
    this.name = "AppError";
    this.status = status;
    this.code = code;
    this.details = details;
  }

  static badRequest(message: string, details?: unknown) {
    return new AppError(400, "bad_request", message, details);
  }

  static unauthorized(message = "Nepieciešama autentifikācija") {
    return new AppError(401, "unauthorized", message);
  }

  static forbidden(message = "Piekļuve liegta") {
    return new AppError(403, "forbidden", message);
  }

  static notFound(message = "Resurss nav atrasts") {
    return new AppError(404, "not_found", message);
  }

  static conflict(message: string) {
    return new AppError(409, "conflict", message);
  }

  static upstream(message: string, details?: unknown) {
    return new AppError(502, "upstream_error", message, details);
  }

  static unavailable(message: string) {
    return new AppError(503, "service_unavailable", message);
  }
}
