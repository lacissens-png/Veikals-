import React, { useCallback, useState } from "react";
import { ScrollView, StyleSheet, Switch, Text, View } from "react-native";
import { SafeAreaView } from "react-native-safe-area-context";
import { useFocusEffect } from "@react-navigation/native";
import { api, ApiError } from "../api/client";
import { Badge, Button, Card, ErrorView, LoadingView } from "../components";
import { categoryLabel, formatEur, frequencyLabel, theme } from "../theme";
import type { SubscriptionDetail } from "../api/types";
import type { ScreenProps } from "../navigation/types";

/** 5. ekrāns: abonementa vēsture, cena, pogas "Atcelt" / "Pārrunāt". */
export function SubscriptionDetailScreen({
  navigation,
  route,
}: ScreenProps<"SubscriptionDetail">) {
  const { subscriptionId, merchantName } = route.params;

  const [detail, setDetail] = useState<SubscriptionDetail | null>(null);
  const [loading, setLoading] = useState(true);
  const [error, setError] = useState<string | null>(null);
  const [flagPending, setFlagPending] = useState(false);

  const load = useCallback(async () => {
    setError(null);
    try {
      setDetail(await api.getSubscription(subscriptionId));
    } catch (err) {
      setError(
        err instanceof ApiError ? err.message : "Neizdevās ielādēt datus.",
      );
    } finally {
      setLoading(false);
    }
  }, [subscriptionId]);

  useFocusEffect(
    useCallback(() => {
      void load();
    }, [load]),
  );

  async function toggleUnwanted(value: boolean) {
    if (!detail) return;

    setFlagPending(true);
    // Optimistiska atjaunināšana — slēdzis reaģē uzreiz.
    setDetail({ ...detail, isFlaggedUnwanted: value });

    try {
      await api.setUnwanted(subscriptionId, value);
    } catch (err) {
      setDetail({ ...detail, isFlaggedUnwanted: !value });
      setError(
        err instanceof ApiError ? err.message : "Neizdevās saglabāt atzīmi.",
      );
    } finally {
      setFlagPending(false);
    }
  }

  if (loading) {
    return (
      <SafeAreaView style={styles.safe} edges={["bottom"]}>
        <LoadingView label="Ielādējam abonementa datus..." />
      </SafeAreaView>
    );
  }

  if (!detail) {
    return (
      <SafeAreaView style={styles.safe} edges={["bottom"]}>
        <View style={styles.content}>
          <ErrorView
            message={error ?? "Abonements nav atrasts."}
            onRetry={() => void load()}
          />
        </View>
      </SafeAreaView>
    );
  }

  const priceChange = detail.priceChangePercent ?? 0;

  return (
    <SafeAreaView style={styles.safe} edges={["bottom"]}>
      <ScrollView contentContainerStyle={styles.content}>
        {error ? <ErrorView message={error} /> : null}

        <Card style={styles.headerCard}>
          <Text style={styles.merchant}>{detail.merchantName}</Text>
          <Text style={styles.amount}>{formatEur(detail.averageAmount)}</Text>
          <Text style={styles.meta}>
            {frequencyLabel(detail.frequency)} • {categoryLabel(detail.category)}
          </Text>
          <Text style={styles.meta}>
            {formatEur(detail.monthlyCost)} mēnesī pēc pārrēķina
          </Text>
        </Card>

        {detail.priceChangeDetected ? (
          <Card style={styles.warnCard}>
            <Badge
              label={`Cena mainījusies ${priceChange > 0 ? "+" : ""}${priceChange.toFixed(1)}%`}
              tone="warning"
            />
            <Text style={styles.warnText}>
              No {formatEur(detail.firstAmount)} uz {formatEur(detail.lastAmount)}
            </Text>
          </Card>
        ) : null}

        <Card>
          <View style={styles.switchRow}>
            <View style={styles.switchLabel}>
              <Text style={styles.switchTitle}>Atzīmēt kā nevēlamu</Text>
              <Text style={styles.switchHint}>
                Palīdz atcerēties, ko gribi atcelt.
              </Text>
            </View>
            <Switch
              value={detail.isFlaggedUnwanted}
              onValueChange={(value) => void toggleUnwanted(value)}
              disabled={flagPending}
              trackColor={{
                false: theme.colors.border,
                true: theme.colors.danger,
              }}
            />
          </View>
        </Card>

        <Card>
          <Text style={styles.sectionTitle}>Fakti</Text>
          <Fact label="Pirmo reizi" value={detail.firstSeen ?? "—"} />
          <Fact label="Pēdējo reizi" value={detail.lastSeen ?? "—"} />
          <Fact
            label="Maksājumu skaits"
            value={detail.occurrences ? String(detail.occurrences) : "—"}
          />
        </Card>

        <Card>
          <Text style={styles.sectionTitle}>Maksājumu vēsture</Text>
          {detail.history.length === 0 ? (
            <Text style={styles.emptyText}>
              Šim abonementam vēl nav saistīto darījumu.
            </Text>
          ) : (
            detail.history.map((item) => (
              <View key={item.id} style={styles.historyRow}>
                <Text style={styles.historyDate}>{item.date}</Text>
                <Text style={styles.historyDesc} numberOfLines={1}>
                  {item.description}
                </Text>
                <Text style={styles.historyAmount}>
                  {formatEur(Math.abs(item.amount))}
                </Text>
              </View>
            ))
          )}
        </Card>

        <View style={styles.actions}>
          <Button
            title="Atcelt abonementu"
            onPress={() =>
              navigation.navigate("DraftAction", {
                subscriptionId,
                merchantName,
                actionType: "cancel",
              })
            }
          />
          <Button
            title="Pārrunāt cenu"
            variant="secondary"
            onPress={() =>
              navigation.navigate("DraftAction", {
                subscriptionId,
                merchantName,
                actionType: "negotiate",
              })
            }
          />
        </View>
      </ScrollView>
    </SafeAreaView>
  );
}

function Fact({ label, value }: { label: string; value: string }) {
  return (
    <View style={styles.factRow}>
      <Text style={styles.factLabel}>{label}</Text>
      <Text style={styles.factValue}>{value}</Text>
    </View>
  );
}

const styles = StyleSheet.create({
  safe: { flex: 1, backgroundColor: theme.colors.background },
  content: { padding: theme.spacing(2), gap: theme.spacing(2) },
  headerCard: { alignItems: "center", paddingVertical: theme.spacing(3) },
  merchant: {
    color: theme.colors.text,
    fontSize: 20,
    fontWeight: "700",
    textAlign: "center",
  },
  amount: {
    color: theme.colors.text,
    fontSize: 34,
    fontWeight: "700",
    marginVertical: theme.spacing(0.5),
  },
  meta: { color: theme.colors.textMuted, fontSize: 14 },
  warnCard: { gap: theme.spacing(1), borderColor: theme.colors.warning },
  warnText: { color: theme.colors.text, fontSize: 14 },
  switchRow: { flexDirection: "row", alignItems: "center" },
  switchLabel: { flex: 1, gap: 2 },
  switchTitle: { color: theme.colors.text, fontSize: 15, fontWeight: "600" },
  switchHint: { color: theme.colors.textMuted, fontSize: 13 },
  sectionTitle: {
    color: theme.colors.text,
    fontSize: 15,
    fontWeight: "600",
    marginBottom: theme.spacing(1),
  },
  factRow: {
    flexDirection: "row",
    justifyContent: "space-between",
    paddingVertical: theme.spacing(0.5),
  },
  factLabel: { color: theme.colors.textMuted, fontSize: 14 },
  factValue: { color: theme.colors.text, fontSize: 14, fontWeight: "500" },
  historyRow: {
    flexDirection: "row",
    alignItems: "center",
    paddingVertical: theme.spacing(0.75),
    borderTopWidth: StyleSheet.hairlineWidth,
    borderTopColor: theme.colors.border,
  },
  historyDate: {
    color: theme.colors.textMuted,
    fontSize: 13,
    width: 88,
  },
  historyDesc: { color: theme.colors.text, fontSize: 14, flex: 1 },
  historyAmount: {
    color: theme.colors.text,
    fontSize: 14,
    fontWeight: "600",
    marginLeft: theme.spacing(1),
  },
  emptyText: { color: theme.colors.textMuted, fontSize: 14 },
  actions: { gap: theme.spacing(1), marginTop: theme.spacing(1) },
});
