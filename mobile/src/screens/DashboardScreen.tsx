import React, { useCallback, useState } from "react";
import {
  FlatList,
  Pressable,
  RefreshControl,
  StyleSheet,
  Text,
  View,
} from "react-native";
import { SafeAreaView } from "react-native-safe-area-context";
import { useFocusEffect } from "@react-navigation/native";
import { api, ApiError } from "../api/client";
import { Badge, Button, Card, ErrorView, LoadingView } from "../components";
import { categoryLabel, formatEur, frequencyLabel, theme } from "../theme";
import type { Subscription, SubscriptionSummary } from "../api/types";
import type { ScreenProps } from "../navigation/types";

/** 4. ekrāns: kopējā abonementu summa un saraksts pa kategorijām. */
export function DashboardScreen({ navigation }: ScreenProps<"Dashboard">) {
  const [summary, setSummary] = useState<SubscriptionSummary | null>(null);
  const [subscriptions, setSubscriptions] = useState<Subscription[]>([]);
  const [loading, setLoading] = useState(true);
  const [refreshing, setRefreshing] = useState(false);
  const [error, setError] = useState<string | null>(null);

  const load = useCallback(async (isRefresh = false) => {
    if (isRefresh) setRefreshing(true);
    setError(null);

    try {
      const result = await api.listSubscriptions();
      setSummary(result.summary);
      setSubscriptions(result.subscriptions);
    } catch (err) {
      setError(
        err instanceof ApiError
          ? err.message
          : "Neizdevās ielādēt abonementus.",
      );
    } finally {
      setLoading(false);
      setRefreshing(false);
    }
  }, []);

  // Pārlādē katru reizi, kad ekrāns kļūst redzams — atzīmes detaļu skatā
  // uzreiz parādās sarakstā.
  useFocusEffect(
    useCallback(() => {
      void load();
    }, [load]),
  );

  if (loading) {
    return (
      <SafeAreaView style={styles.safe} edges={["bottom"]}>
        <LoadingView label="Ielādējam pārskatu..." />
      </SafeAreaView>
    );
  }

  return (
    <SafeAreaView style={styles.safe} edges={["bottom"]}>
      <FlatList
        data={subscriptions}
        keyExtractor={(item) => item.id}
        contentContainerStyle={styles.content}
        refreshControl={
          <RefreshControl
            refreshing={refreshing}
            onRefresh={() => void load(true)}
            tintColor={theme.colors.primary}
          />
        }
        ListHeaderComponent={
          <View style={styles.header}>
            {error ? (
              <ErrorView message={error} onRetry={() => void load()} />
            ) : null}

            <Card style={styles.totalCard}>
              <Text style={styles.totalLabel}>Kopā mēnesī</Text>
              <Text style={styles.totalValue}>
                {formatEur(summary?.monthlyTotal)}
              </Text>
              <Text style={styles.totalSub}>
                {formatEur(summary?.yearlyTotal)} gadā •{" "}
                {summary?.count ?? 0} abonementi
              </Text>

              {summary && summary.priceChangeCount > 0 ? (
                <View style={styles.alertRow}>
                  <Badge
                    label={`${summary.priceChangeCount} cenu izmaiņas`}
                    tone="warning"
                  />
                </View>
              ) : null}
            </Card>

            {summary && summary.byCategory.length > 0 ? (
              <Card>
                <Text style={styles.sectionTitle}>Pa kategorijām</Text>
                {summary.byCategory.map((item) => (
                  <View key={item.category} style={styles.categoryRow}>
                    <Text style={styles.categoryName}>
                      {categoryLabel(item.category)}
                    </Text>
                    <Text style={styles.categoryCount}>
                      {item.count} ×
                    </Text>
                    <Text style={styles.categoryAmount}>
                      {formatEur(item.monthlyTotal)}
                    </Text>
                  </View>
                ))}
              </Card>
            ) : null}

            <Text style={styles.sectionTitle}>Abonementi</Text>
          </View>
        }
        renderItem={({ item }) => (
          <Pressable
            accessibilityRole="button"
            onPress={() =>
              navigation.navigate("SubscriptionDetail", {
                subscriptionId: item.id,
                merchantName: item.merchantName,
              })
            }
            style={({ pressed }) => [
              styles.row,
              pressed && styles.rowPressed,
              item.isFlaggedUnwanted && styles.rowFlagged,
            ]}
          >
            <View style={styles.rowMain}>
              <Text style={styles.rowName} numberOfLines={1}>
                {item.merchantName}
              </Text>
              <Text style={styles.rowMeta}>
                {categoryLabel(item.category)} •{" "}
                {frequencyLabel(item.frequency)}
              </Text>
              <View style={styles.rowBadges}>
                {item.priceChangeDetected ? (
                  <Badge
                    label={`Cena ${
                      (item.priceChangePercent ?? 0) > 0 ? "+" : ""
                    }${(item.priceChangePercent ?? 0).toFixed(1)}%`}
                    tone="warning"
                  />
                ) : null}
                {item.isFlaggedUnwanted ? (
                  <Badge label="Nevēlams" tone="danger" />
                ) : null}
              </View>
            </View>
            <Text style={styles.rowAmount}>
              {formatEur(item.averageAmount)}
            </Text>
          </Pressable>
        )}
        ListEmptyComponent={
          error ? null : (
            <Card>
              <Text style={styles.emptyTitle}>Abonementi vēl nav atrasti</Text>
              <Text style={styles.emptyText}>
                Savieno bankas kontu un palaid analīzi, lai redzētu savus
                atkārtotos maksājumus.
              </Text>
              <View style={styles.emptyAction}>
                <Button
                  title="Savienot banku"
                  onPress={() => navigation.navigate("BankConnect")}
                />
              </View>
            </Card>
          )
        }
        ListFooterComponent={
          <View style={styles.footer}>
            <Button
              title="Atjaunot analīzi"
              variant="secondary"
              onPress={() => navigation.navigate("Loading", { step: "sync" })}
            />
          </View>
        }
      />
    </SafeAreaView>
  );
}

const styles = StyleSheet.create({
  safe: { flex: 1, backgroundColor: theme.colors.background },
  content: { padding: theme.spacing(2), gap: theme.spacing(1) },
  header: { gap: theme.spacing(2), marginBottom: theme.spacing(1) },
  totalCard: { alignItems: "center", paddingVertical: theme.spacing(3) },
  totalLabel: { color: theme.colors.textMuted, fontSize: 14 },
  totalValue: {
    color: theme.colors.text,
    fontSize: 40,
    fontWeight: "700",
    marginVertical: theme.spacing(0.5),
  },
  totalSub: { color: theme.colors.textMuted, fontSize: 14 },
  alertRow: { marginTop: theme.spacing(1.5) },
  sectionTitle: {
    color: theme.colors.text,
    fontSize: 16,
    fontWeight: "600",
    marginBottom: theme.spacing(1),
  },
  categoryRow: {
    flexDirection: "row",
    alignItems: "center",
    paddingVertical: theme.spacing(0.75),
  },
  categoryName: { color: theme.colors.text, fontSize: 15, flex: 1 },
  categoryCount: {
    color: theme.colors.textMuted,
    fontSize: 13,
    marginRight: theme.spacing(1.5),
  },
  categoryAmount: {
    color: theme.colors.text,
    fontSize: 15,
    fontWeight: "600",
  },
  row: {
    flexDirection: "row",
    alignItems: "center",
    backgroundColor: theme.colors.surface,
    borderRadius: theme.radius.md,
    borderWidth: 1,
    borderColor: theme.colors.border,
    padding: theme.spacing(2),
    marginBottom: theme.spacing(1),
  },
  rowPressed: { opacity: 0.7 },
  rowFlagged: { borderColor: theme.colors.danger },
  rowMain: { flex: 1, gap: 4 },
  rowName: { color: theme.colors.text, fontSize: 16, fontWeight: "600" },
  rowMeta: { color: theme.colors.textMuted, fontSize: 13 },
  rowBadges: { flexDirection: "row", gap: theme.spacing(0.75), marginTop: 4 },
  rowAmount: {
    color: theme.colors.text,
    fontSize: 17,
    fontWeight: "700",
    marginLeft: theme.spacing(1.5),
  },
  emptyTitle: {
    color: theme.colors.text,
    fontSize: 16,
    fontWeight: "600",
    marginBottom: theme.spacing(1),
  },
  emptyText: { color: theme.colors.textMuted, fontSize: 14, lineHeight: 21 },
  emptyAction: { marginTop: theme.spacing(2) },
  footer: { marginTop: theme.spacing(2), gap: theme.spacing(1) },
});
