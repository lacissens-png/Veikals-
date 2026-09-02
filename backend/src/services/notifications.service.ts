import { Expo, type ExpoPushMessage } from "expo-server-sdk";
import { logger } from "../lib/logger.js";

/**
 * Push paziņojumi par cenu izmaiņām (MVP funkcija #9).
 *
 * Expo push serviss nepieprasa atslēgu neautentificētiem sūtījumiem, tāpēc
 * MVP tas darbojas uzreiz. Ja lietotājam nav reģistrēta tokena, funkcija
 * klusē — tā nekad neizraisa analīzes neizdošanos.
 */
const expo = new Expo();

export interface PriceChangeNotice {
  merchantName: string;
  oldAmount: number;
  newAmount: number;
  changePercent: number;
}

export async function notifyPriceChanges(
  pushToken: string | null,
  changes: PriceChangeNotice[],
): Promise<void> {
  if (!pushToken || changes.length === 0) {
    return;
  }

  if (!Expo.isExpoPushToken(pushToken)) {
    logger.warn("Nederīgs Expo push token — paziņojums izlaists");
    return;
  }

  const messages: ExpoPushMessage[] = changes.map((change) => ({
    to: pushToken,
    sound: "default",
    title: `${change.merchantName} cena mainījusies`,
    body:
      `${change.oldAmount.toFixed(2)} € → ${change.newAmount.toFixed(2)} € ` +
      `(${change.changePercent > 0 ? "+" : ""}${change.changePercent.toFixed(1)}%)`,
    data: { merchantName: change.merchantName, type: "price_change" },
  }));

  try {
    for (const chunk of expo.chunkPushNotifications(messages)) {
      await expo.sendPushNotificationsAsync(chunk);
    }
    logger.info("Push paziņojumi nosūtīti", { count: messages.length });
  } catch (error) {
    // Paziņojuma neizdošanās nedrīkst apturēt analīzi.
    logger.error("Neizdevās nosūtīt push paziņojumus", {
      message: error instanceof Error ? error.message : String(error),
    });
  }
}
