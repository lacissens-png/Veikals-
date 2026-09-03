import { Platform } from "react-native";
import Constants from "expo-constants";
import * as Device from "expo-device";
import * as Notifications from "expo-notifications";
import { api } from "./api/client";

/**
 * Push paziņojumi par cenu izmaiņām (MVP funkcija #9).
 *
 * Emulatorā/simulatorā push nedarbojas, tāpēc tur klusi izlaižam.
 */
Notifications.setNotificationHandler({
  handleNotification: async () => ({
    shouldShowBanner: true,
    shouldShowList: true,
    shouldPlaySound: false,
    shouldSetBadge: false,
  }),
});

export async function registerForPushNotifications(): Promise<string | null> {
  if (!Device.isDevice) {
    return null;
  }

  if (Platform.OS === "android") {
    await Notifications.setNotificationChannelAsync("default", {
      name: "Paziņojumi",
      importance: Notifications.AndroidImportance.DEFAULT,
    });
  }

  const existing = await Notifications.getPermissionsAsync();
  let status = existing.status;

  if (status !== "granted") {
    const requested = await Notifications.requestPermissionsAsync();
    status = requested.status;
  }

  if (status !== "granted") {
    return null;
  }

  // projectId nāk no EAS konfigurācijas; bez tā Expo nevar izsniegt tokenu.
  const projectId =
    Constants.expoConfig?.extra?.eas?.projectId ??
    Constants.easConfig?.projectId;

  if (!projectId) {
    // Bez EAS projekta MVP vienkārši strādā bez push — nav iemesla krist.
    return null;
  }

  const token = await Notifications.getExpoPushTokenAsync({ projectId });

  try {
    await api.registerPushToken(token.data);
  } catch {
    // Tokena reģistrācija nav kritiska aplikācijas darbībai.
    return token.data;
  }

  return token.data;
}
