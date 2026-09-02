import React, { useCallback, useState } from "react";
import { Alert, ScrollView, StyleSheet, Text, View } from "react-native";
import { SafeAreaView } from "react-native-safe-area-context";
import { useFocusEffect } from "@react-navigation/native";
import { api, ApiError, API_BASE_URL } from "../api/client";
import { useAuth } from "../auth/AuthContext";
import { Badge, Button, Card, ErrorView } from "../components";
import { registerForPushNotifications } from "../notifications";
import { theme } from "../theme";
import type { BankConnection } from "../api/types";
import type { ScreenProps } from "../navigation/types";

/** 7. ekrāns: konta iestatījumi, bankas atvienošana, datu dzēšana. */
export function SettingsScreen({ navigation }: ScreenProps<"Settings">) {
  const { user, signOut } = useAuth();
  const [connections, setConnections] = useState<BankConnection[]>([]);
  const [error, setError] = useState<string | null>(null);
  const [busy, setBusy] = useState<string | null>(null);
  const [pushStatus, setPushStatus] = useState<string | null>(null);

  const load = useCallback(async () => {
    setError(null);
    try {
      const result = await api.listConnections();
      setConnections(result.connections);
    } catch (err) {
      setError(
        err instanceof ApiError
          ? err.message
          : "Neizdevās ielādēt savienojumus.",
      );
    }
  }, []);

  useFocusEffect(
    useCallback(() => {
      void load();
    }, [load]),
  );

  function confirmDisconnect(connection: BankConnection) {
    Alert.alert(
      "Atvienot banku?",
      "Savienojums un visi ielādētie darījumi tiks dzēsti. Abonementu saraksts paliks, līdz palaidīsi jaunu analīzi.",
      [
        { text: "Atcelt", style: "cancel" },
        {
          text: "Atvienot",
          style: "destructive",
          onPress: () => void disconnect(connection.id),
        },
      ],
    );
  }

  async function disconnect(id: string) {
    setBusy(id);
    setError(null);
    try {
      await api.disconnectBank(id);
      await load();
    } catch (err) {
      setError(
        err instanceof ApiError ? err.message : "Neizdevās atvienot banku.",
      );
    } finally {
      setBusy(null);
    }
  }

  function confirmDelete() {
    Alert.alert(
      "Dzēst kontu?",
      "Tiks neatgriezeniski dzēsts konts, visi bankas savienojumi, darījumi, abonementi un melnraksti.",
      [
        { text: "Atcelt", style: "cancel" },
        {
          text: "Dzēst visu",
          style: "destructive",
          onPress: () => void deleteAccount(),
        },
      ],
    );
  }

  async function deleteAccount() {
    setBusy("delete");
    setError(null);
    try {
      await api.deleteAccount();
      await signOut();
    } catch (err) {
      setError(
        err instanceof ApiError ? err.message : "Neizdevās dzēst kontu.",
      );
      setBusy(null);
    }
  }

  async function enablePush() {
    setBusy("push");
    setPushStatus(null);
    try {
      const token = await registerForPushNotifications();
      setPushStatus(
        token
          ? "Paziņojumi ieslēgti. Ziņosim, ja kāda abonementa cena mainīsies."
          : "Paziņojumi nav pieejami šajā ierīcē (nepieciešama fiziska ierīce un EAS projekts).",
      );
    } catch {
      setPushStatus("Neizdevās ieslēgt paziņojumus.");
    } finally {
      setBusy(null);
    }
  }

  return (
    <SafeAreaView style={styles.safe} edges={["bottom"]}>
      <ScrollView contentContainerStyle={styles.content}>
        {error ? <ErrorView message={error} onRetry={() => void load()} /> : null}

        <Card>
          <Text style={styles.sectionTitle}>Konts</Text>
          <Text style={styles.value}>{user?.email ?? "—"}</Text>
          <Text style={styles.hint}>Serveris: {API_BASE_URL}</Text>
        </Card>

        <Card>
          <Text style={styles.sectionTitle}>Bankas savienojumi</Text>
          {connections.length === 0 ? (
            <>
              <Text style={styles.hint}>Nav neviena savienojuma.</Text>
              <View style={styles.action}>
                <Button
                  title="Savienot banku"
                  onPress={() => navigation.navigate("BankConnect")}
                />
              </View>
            </>
          ) : (
            connections.map((connection) => (
              <View key={connection.id} style={styles.connectionRow}>
                <View style={styles.connectionInfo}>
                  <Text style={styles.value}>
                    {connection.providerAccountId ?? "Konts"}
                  </Text>
                  <Text style={styles.hint}>
                    {connection.lastSyncedAt
                      ? `Pēdējā sinhronizācija: ${connection.lastSyncedAt.slice(0, 10)}`
                      : "Vēl nav sinhronizēts"}
                  </Text>
                  <Badge
                    label={connection.status}
                    tone={connection.status === "active" ? "success" : "neutral"}
                  />
                </View>
                <Button
                  title="Atvienot"
                  variant="secondary"
                  loading={busy === connection.id}
                  onPress={() => confirmDisconnect(connection)}
                />
              </View>
            ))
          )}
        </Card>

        <Card>
          <Text style={styles.sectionTitle}>Paziņojumi</Text>
          <Text style={styles.hint}>
            Saņem push paziņojumu, kad kāda abonementa cena mainās.
          </Text>
          {pushStatus ? <Text style={styles.status}>{pushStatus}</Text> : null}
          <View style={styles.action}>
            <Button
              title="Ieslēgt paziņojumus"
              variant="secondary"
              loading={busy === "push"}
              onPress={() => void enablePush()}
            />
          </View>
        </Card>

        <Card>
          <Text style={styles.sectionTitle}>Dati</Text>
          <Text style={styles.hint}>
            Dzēšana ir neatgriezeniska un aizvāc visus tavus datus no mūsu
            serveriem.
          </Text>
          <View style={styles.action}>
            <Button
              title="Izrakstīties"
              variant="secondary"
              onPress={() => void signOut()}
            />
          </View>
          <View style={styles.action}>
            <Button
              title="Dzēst kontu un visus datus"
              variant="danger"
              loading={busy === "delete"}
              onPress={confirmDelete}
            />
          </View>
        </Card>
      </ScrollView>
    </SafeAreaView>
  );
}

const styles = StyleSheet.create({
  safe: { flex: 1, backgroundColor: theme.colors.background },
  content: { padding: theme.spacing(2), gap: theme.spacing(2) },
  sectionTitle: {
    color: theme.colors.text,
    fontSize: 15,
    fontWeight: "600",
    marginBottom: theme.spacing(1),
  },
  value: { color: theme.colors.text, fontSize: 15 },
  hint: {
    color: theme.colors.textMuted,
    fontSize: 13,
    lineHeight: 19,
    marginTop: 2,
  },
  status: {
    color: theme.colors.success,
    fontSize: 13,
    marginTop: theme.spacing(1),
  },
  action: { marginTop: theme.spacing(1.5) },
  connectionRow: {
    flexDirection: "row",
    alignItems: "center",
    gap: theme.spacing(1.5),
    paddingVertical: theme.spacing(1),
  },
  connectionInfo: { flex: 1, gap: 4 },
});
