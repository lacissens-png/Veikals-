import React, { useCallback, useState } from "react";
import {
  RefreshControl,
  ScrollView,
  StyleSheet,
  Text,
  View,
} from "react-native";
import { SafeAreaView } from "react-native-safe-area-context";
import { useFocusEffect } from "@react-navigation/native";
import { api, ApiError } from "../api/client";
import { Badge, Button, Card, ErrorView, LoadingView } from "../components";
import { formatEur, theme } from "../theme";
import type { EmailFinding, FindingsResponse } from "../api/types";
import type { ScreenProps } from "../navigation/types";

const KIND_LABELS: Record<string, string> = {
  trial_ending: "Izmēģinājums beidzas",
  price_change: "Cenas maiņa",
  renewal: "Automātiska atjaunošana",
  invoice: "Rēķins",
  suspicious: "Aizdomīga vēstule",
};

/** Cik dienas līdz notikumam — tas ir tas, kas lietotājam jāzina uzreiz. */
function daysUntil(date: string | null): number | null {
  if (!date) return null;
  const target = new Date(`${date}T00:00:00.000Z`).getTime();
  const today = new Date();
  today.setUTCHours(0, 0, 0, 0);
  return Math.round((target - today.getTime()) / 86_400_000);
}

function Countdown({ date }: { date: string | null }) {
  const days = daysUntil(date);
  if (days === null) return null;

  if (days < 0) return <Text style={styles.meta}>Jau noticis</Text>;
  if (days === 0) return <Text style={styles.urgent}>Šodien</Text>;
  if (days <= 7) return <Text style={styles.urgent}>Pēc {days} dienām</Text>;
  return <Text style={styles.meta}>Pēc {days} dienām</Text>;
}

/**
 * Brīdinājumu ekrāns: krāpšana augšā, gaidāmie maksājumi zemāk.
 *
 * Krāpnieciskās vēstules paliek pastkastītē — lietotne tās tikai parāda un
 * paskaidro, kāpēc tās izskatās aizdomīgi.
 */
export function AlertsScreen({ navigation }: ScreenProps<"Alerts">) {
  const [data, setData] = useState<FindingsResponse | null>(null);
  const [loading, setLoading] = useState(true);
  const [refreshing, setRefreshing] = useState(false);
  const [syncing, setSyncing] = useState(false);
  const [error, setError] = useState<string | null>(null);
  const [busyId, setBusyId] = useState<string | null>(null);

  const load = useCallback(async (isRefresh = false) => {
    if (isRefresh) setRefreshing(true);
    setError(null);
    try {
      setData(await api.listFindings());
    } catch (err) {
      setError(err instanceof ApiError ? err.message : "Neizdevās ielādēt brīdinājumus.");
    } finally {
      setLoading(false);
      setRefreshing(false);
    }
  }, []);

  useFocusEffect(
    useCallback(() => {
      void load();
    }, [load]),
  );

  async function sync() {
    setSyncing(true);
    setError(null);
    try {
      await api.syncEmail();
      await load();
    } catch (err) {
      setError(err instanceof ApiError ? err.message : "Neizdevās pārbaudīt pastu.");
    } finally {
      setSyncing(false);
    }
  }

  async function dismiss(finding: EmailFinding) {
    setBusyId(finding.id);
    try {
      await api.setFindingStatus(finding.id, "dismissed");
      await load();
    } catch (err) {
      setError(err instanceof ApiError ? err.message : "Neizdevās atmest.");
    } finally {
      setBusyId(null);
    }
  }

  if (loading) {
    return (
      <SafeAreaView style={styles.safe} edges={["bottom"]}>
        <LoadingView label="Ielādējam brīdinājumus..." />
      </SafeAreaView>
    );
  }

  const suspicious = data?.suspicious ?? [];
  const upcoming = data?.upcoming ?? [];

  return (
    <SafeAreaView style={styles.safe} edges={["bottom"]}>
      <ScrollView
        contentContainerStyle={styles.content}
        refreshControl={
          <RefreshControl
            refreshing={refreshing}
            onRefresh={() => void load(true)}
            tintColor={theme.colors.primary}
          />
        }
      >
        {error ? <ErrorView message={error} onRetry={() => void load()} /> : null}

        {suspicious.length > 0 ? (
          <>
            <Text style={styles.sectionTitle}>
              Aizdomīgas vēstules ({suspicious.length})
            </Text>
            <Text style={styles.sectionNote}>
              Šīs vēstules paliek tavā pastkastītē — mēs tās neaiztiekam. Nespied
              saites un neievadi datus.
            </Text>

            {suspicious.map((finding) => (
              <Card key={finding.id} style={styles.dangerCard}>
                <View style={styles.rowTop}>
                  <Badge label="Iespējama krāpšana" tone="danger" />
                  <Text style={styles.date}>{finding.receivedAt}</Text>
                </View>

                <Text style={styles.merchant}>{finding.merchantName}</Text>
                {finding.senderAddress ? (
                  <Text style={styles.sender}>{finding.senderAddress}</Text>
                ) : null}
                <Text style={styles.summary}>{finding.summary}</Text>

                {finding.riskReasons.length > 0 ? (
                  <View style={styles.reasons}>
                    <Text style={styles.reasonsTitle}>Kāpēc tas izskatās aizdomīgi</Text>
                    {finding.riskReasons.map((reason) => (
                      <Text key={reason} style={styles.reason}>
                        • {reason}
                      </Text>
                    ))}
                  </View>
                ) : null}

                <View style={styles.action}>
                  <Button
                    title="Sapratu, paslēpt"
                    variant="secondary"
                    loading={busyId === finding.id}
                    onPress={() => void dismiss(finding)}
                  />
                </View>
              </Card>
            ))}
          </>
        ) : null}

        <Text style={styles.sectionTitle}>Gaidāmie maksājumi ({upcoming.length})</Text>

        {upcoming.length === 0 ? (
          <Card>
            <Text style={styles.summary}>
              Pagaidām nekā. Pārbaudi pastu, lai atrastu izmēģinājumus, kas drīz
              sāks maksāt, un paziņojumus par cenu izmaiņām.
            </Text>
          </Card>
        ) : (
          upcoming.map((finding) => (
            <Card key={finding.id}>
              <View style={styles.rowTop}>
                <Badge
                  label={KIND_LABELS[finding.kind] ?? finding.kind}
                  tone={finding.kind === "trial_ending" ? "warning" : "neutral"}
                />
                <Countdown date={finding.effectiveDate} />
              </View>

              <View style={styles.rowMain}>
                <Text style={styles.merchant}>{finding.merchantName}</Text>
                {finding.amount !== null ? (
                  <Text style={styles.amount}>{formatEur(finding.amount)}</Text>
                ) : null}
              </View>

              <Text style={styles.summary}>{finding.summary}</Text>

              <View style={styles.action}>
                <Button
                  title="Atmest"
                  variant="secondary"
                  loading={busyId === finding.id}
                  onPress={() => void dismiss(finding)}
                />
              </View>
            </Card>
          ))
        )}

        <View style={styles.footer}>
          <Button
            title="Pārbaudīt pastu tagad"
            loading={syncing}
            onPress={() => void sync()}
          />
          <Button
            title="Iestatījumi"
            variant="secondary"
            onPress={() => navigation.navigate("Settings")}
          />
        </View>
      </ScrollView>
    </SafeAreaView>
  );
}

const styles = StyleSheet.create({
  safe: { flex: 1, backgroundColor: theme.colors.background },
  content: { padding: theme.spacing(2), gap: theme.spacing(1.5) },
  sectionTitle: {
    color: theme.colors.text,
    fontSize: 17,
    fontWeight: "700",
    marginTop: theme.spacing(1),
  },
  sectionNote: {
    color: theme.colors.textMuted,
    fontSize: 13,
    lineHeight: 19,
    marginBottom: theme.spacing(0.5),
  },
  dangerCard: { borderColor: theme.colors.danger },
  rowTop: {
    flexDirection: "row",
    alignItems: "center",
    justifyContent: "space-between",
    marginBottom: theme.spacing(1),
  },
  rowMain: {
    flexDirection: "row",
    alignItems: "center",
    justifyContent: "space-between",
  },
  merchant: { color: theme.colors.text, fontSize: 16, fontWeight: "600", flex: 1 },
  sender: { color: theme.colors.textMuted, fontSize: 12, marginBottom: 4 },
  amount: {
    color: theme.colors.text,
    fontSize: 16,
    fontWeight: "700",
    marginLeft: theme.spacing(1),
  },
  summary: {
    color: theme.colors.textMuted,
    fontSize: 14,
    lineHeight: 20,
    marginTop: 4,
  },
  reasons: {
    backgroundColor: "rgba(255, 95, 95, 0.08)",
    borderRadius: theme.radius.sm,
    padding: theme.spacing(1.5),
    marginTop: theme.spacing(1.5),
    gap: 4,
  },
  reasonsTitle: {
    color: theme.colors.danger,
    fontSize: 13,
    fontWeight: "600",
    marginBottom: 2,
  },
  reason: { color: theme.colors.text, fontSize: 13, lineHeight: 19 },
  date: { color: theme.colors.textMuted, fontSize: 12 },
  urgent: { color: theme.colors.warning, fontSize: 13, fontWeight: "600" },
  meta: { color: theme.colors.textMuted, fontSize: 13 },
  action: { marginTop: theme.spacing(1.5) },
  footer: { marginTop: theme.spacing(2), gap: theme.spacing(1) },
});
