import assert from "node:assert/strict";
import { describe, it } from "node:test";
import { z } from "zod";
import { validateBody } from "./validate.js";

/** Minimāls Express pieprasījuma aizstājējs testam. */
function run(schema: z.ZodType, body: unknown) {
  const req = { body } as never as { body: unknown };
  let error: unknown = null;
  let called = false;

  validateBody(schema)(req as never, {} as never, ((err?: unknown) => {
    called = true;
    error = err ?? null;
  }) as never);

  return { req, error, called };
}

describe("validateBody", () => {
  const optionalSchema = z.object({ redirectUrl: z.string().optional() });

  it("pieņem POST bez ķermeņa, ja visi lauki ir neobligāti", () => {
    // express.json() atstāj undefined, ja Content-Type nav JSON vai ķermeņa nav.
    // Bez šī /api/bank/connect atgrieztu 400 klientam, kas neko nesūta.
    const { error, req } = run(optionalSchema, undefined);
    assert.equal(error, null);
    assert.deepEqual(req.body, {});
  });

  it("pieņem tukšu objektu", () => {
    const { error } = run(optionalSchema, {});
    assert.equal(error, null);
  });

  it("pieņem derīgu ķermeni un aizvieto to ar apstrādāto", () => {
    const { error, req } = run(optionalSchema, { redirectUrl: "exp://x/--/cb" });
    assert.equal(error, null);
    assert.deepEqual(req.body, { redirectUrl: "exp://x/--/cb" });
  });

  it("noraida nederīgu tipu", () => {
    const { error } = run(optionalSchema, { redirectUrl: 42 });
    assert.ok(error instanceof z.ZodError);
  });

  it("noraida trūkstošu obligātu lauku", () => {
    const required = z.object({ status: z.enum(["copied", "dismissed"]) });
    const { error } = run(required, {});
    assert.ok(error instanceof z.ZodError);
  });
});
