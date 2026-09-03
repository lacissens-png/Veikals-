/**
 * Backend adreses noteikšana.
 *
 * Bez atkarībām no expo-constants un react-native, lai to varētu pārbaudīt ar
 * testiem. Šī ir loģika, kas izlemj, vai lietotne vispār sarunājas ar backend —
 * būvētā APK to pārbaudīt vairs nevar, tāpēc testi ir vienīgā aizsardzība.
 */

/** Backend ports izstrādē. Metro klausās 8081, backend — 4000. */
export const API_PORT = 4000;

/**
 * Izvelk datora IP no Expo izstrādes servera adreses.
 *
 * Kad lietotne darbojas Expo Go, tā jau zina, no kurienes ielādējās —
 * piemēram "192.168.1.10:8081". Tas ir tas pats dators, kur darbojas backend.
 *
 * Atgriež null, ja saimniekdatora nav vai tas ir localhost (emulators vai
 * pārlūks — tur der noklusējumi).
 */
export function hostFromExpo(hostUri: string | undefined): string | null {
  if (!hostUri) return null;

  const host = hostUri.split("/")[0]?.split(":")[0];
  if (!host || host === "localhost" || host === "127.0.0.1") return null;

  return host;
}

export interface ApiUrlInput {
  explicitUrl?: string | undefined;
  expoHostUri?: string | undefined;
  platform: string;
  /** Izstrādes režīms. Padots atsevišķi, lai neatkarātos no globālā __DEV__. */
  isDev: boolean;
}

export function resolveApiBaseUrl(input: ApiUrlInput): string {
  // 1. Skaidri norādīts — vienmēr uzvar. Būvētā lietotnē tas ir vienīgais ceļš.
  if (input.explicitUrl) {
    return input.explicitUrl;
  }

  // Būvētā lietotnē izstrādes servera nav. Atkāpšanās uz localhost vai
  // emulatora adresi nozīmētu, ka lietotne klusi nesavienojas un kļūda izskatās
  // pēc tīkla problēmas. Labāk pateikt, kas trūkst.
  if (!input.isDev) {
    throw new Error(
      "EXPO_PUBLIC_API_URL nav iestatīts. Būvētai lietotnei backend adrese " +
        "jānorāda būvēšanas laikā — skat. eas.json profilu.",
    );
  }

  // 2. Fiziska ierīce Expo Go: tas pats dators, no kura ielādējās lietotne.
  const expoHost = hostFromExpo(input.expoHostUri);
  if (expoHost) {
    return `http://${expoHost}:${API_PORT}`;
  }

  // 3. Android emulators sasniedz resursdatoru caur šo īpašo adresi.
  if (input.platform === "android") {
    return `http://10.0.2.2:${API_PORT}`;
  }

  // 4. iOS simulators un pārlūks.
  return `http://localhost:${API_PORT}`;
}
