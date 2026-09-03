import assert from "node:assert/strict";
import { describe, it } from "node:test";
import { hostFromExpo, resolveApiBaseUrl } from "./apiUrl.js";

const dev = { platform: "android", isDev: true };
const prod = { platform: "android", isDev: false };

describe("hostFromExpo", () => {
  it("izvelk IP no Expo Go adreses", () => {
    assert.equal(hostFromExpo("192.168.1.10:8081"), "192.168.1.10");
  });

  it("apstrādā adresi ar ceļu", () => {
    assert.equal(hostFromExpo("10.0.0.5:8081/--/bank-callback"), "10.0.0.5");
  });

  it("localhost nav derīgs saimniekdators", () => {
    // Uz telefona localhost norāda uz pašu telefonu — tur backend nav.
    assert.equal(hostFromExpo("localhost:8081"), null);
    assert.equal(hostFromExpo("127.0.0.1:8081"), null);
  });

  it("tukša vērtība dod null", () => {
    assert.equal(hostFromExpo(undefined), null);
    assert.equal(hostFromExpo(""), null);
  });
});

describe("resolveApiBaseUrl — būvēta lietotne", () => {
  it("lieto būvēšanas laikā norādīto adresi", () => {
    assert.equal(
      resolveApiBaseUrl({ ...prod, explicitUrl: "https://api.piemers.lv" }),
      "https://api.piemers.lv",
    );
  });

  it("bez adreses met skaidru kļūdu, nevis klusi nesavienojas", () => {
    // Šis ir viss šī moduļa jēgas pamats: bez tā APK atkāptos uz emulatora
    // adresi 10.0.2.2 un uz īsta telefona klusi nestrādātu.
    assert.throws(
      () => resolveApiBaseUrl(prod),
      /EXPO_PUBLIC_API_URL nav iestatīts/,
    );
  });

  it("neatkāpjas uz emulatora adresi būvētā lietotnē", () => {
    let message = "";
    try {
      resolveApiBaseUrl({ ...prod, expoHostUri: "192.168.1.10:8081" });
    } catch (error) {
      message = (error as Error).message;
    }
    assert.ok(message.includes("EXPO_PUBLIC_API_URL"));
  });
});

describe("resolveApiBaseUrl — izstrāde", () => {
  it("Expo Go uz telefona: ņem datora IP", () => {
    assert.equal(
      resolveApiBaseUrl({ ...dev, expoHostUri: "192.168.1.10:8081" }),
      "http://192.168.1.10:4000",
    );
  });

  it("skaidra adrese uzvar arī izstrādē", () => {
    assert.equal(
      resolveApiBaseUrl({
        ...dev,
        explicitUrl: "http://192.168.1.99:4000",
        expoHostUri: "192.168.1.10:8081",
      }),
      "http://192.168.1.99:4000",
    );
  });

  it("Android emulators lieto 10.0.2.2", () => {
    assert.equal(
      resolveApiBaseUrl({ ...dev, expoHostUri: "localhost:8081" }),
      "http://10.0.2.2:4000",
    );
  });

  it("iOS simulators un pārlūks lieto localhost", () => {
    assert.equal(
      resolveApiBaseUrl({ platform: "ios", isDev: true }),
      "http://localhost:4000",
    );
    assert.equal(
      resolveApiBaseUrl({ platform: "web", isDev: true }),
      "http://localhost:4000",
    );
  });
});
