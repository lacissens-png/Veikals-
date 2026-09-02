import React, { useState } from "react";
import { ScrollView, StyleSheet, Text, View } from "react-native";
import { SafeAreaView } from "react-native-safe-area-context";
import * as WebBrowser from "expo-web-browser";
import { api, ApiError } from "../api/client";
import { Button, Card, ErrorView } from "../components";
import { theme } from "../theme";
import type { ScreenProps } from "../navigation/types";

/**
 * 2. ekrāns: bankas izvēle un novirzīšana uz Enable Banking OAuth.
 *
 * MVP darbojas ar vienu valsti (NL). Banku saraksts ir informatīvs — pati
 * izvēle notiek Enable Banking pusē, kur lietotājs redz visas pieejamās bankas.
 */
const BANKS = ["ING", "Rabobank", "ABN AMRO", "bunq", "SNS Bank", "ASN Bank"];

export function BankConnectScreen({ navigation }: ScreenProps<"BankConnect">) {
  const [loading, setLoading] = useState(false);
  const [error, setError] = useState<string | null>(null);

  async function connect() {
    setError(null);
    setLoading(true);

    try {
      const { authorizationUrl } = await api.connectBank();

      // openAuthSessionAsync atgriežas, kad banka novirza uz mūsu deep link.
      const result = await WebBrowser.openAuthSessionAsync(
        authorizationUrl,
        "abonementi://bank-callback",
      );

      if (result.type === "success") {
        const url = new URL(result.url);
        if (url.searchParams.get("status") === "error") {
          setError(
            "Banka noraidīja savienojumu: " +
              (url.searchParams.get("reason") ?? "nezināms iemesls"),
          );
          return;
        }
        navigation.replace("Loading", { step: "sync" });
        return;
      }

      if (result.type === "cancel" || result.type === "dismiss") {
        setError("Savienojums tika atcelts. Vari mēģināt vēlreiz.");
        return;
      }

      setError("Neizdevās pabeigt bankas autorizāciju.");
    } catch (err) {
      setError(
        err instanceof ApiError
          ? err.message
          : "Neizdevās sākt bankas savienojumu.",
      );
    } finally {
      setLoading(false);
    }
  }

  return (
    <SafeAreaView style={styles.safe} edges={["bottom"]}>
      <ScrollView contentContainerStyle={styles.content}>
        <Text style={styles.title}>Savieno savu banku</Text>
        <Text style={styles.subtitle}>
          Tu tiksi novirzīts uz savas bankas drošo pieteikšanās lapu. Mēs
          nekad neredzam tavu bankas paroli.
        </Text>

        <Card>
          <Text style={styles.cardTitle}>Atbalstītās bankas (Nīderlande)</Text>
          <View style={styles.bankList}>
            {BANKS.map((bank) => (
              <View key={bank} style={styles.bankChip}>
                <Text style={styles.bankText}>{bank}</Text>
              </View>
            ))}
          </View>
          <Text style={styles.note}>
            Precīzu banku vari izvēlēties nākamajā solī.
          </Text>
        </Card>

        {error ? <ErrorView message={error} onRetry={connect} /> : null}

        <Button
          title="Savienot kontu"
          onPress={connect}
          loading={loading}
        />
        <Button
          title="Atpakaļ"
          variant="secondary"
          disabled={loading}
          onPress={() => navigation.goBack()}
        />
      </ScrollView>
    </SafeAreaView>
  );
}

const styles = StyleSheet.create({
  safe: { flex: 1, backgroundColor: theme.colors.background },
  content: { padding: theme.spacing(3), gap: theme.spacing(2) },
  title: { color: theme.colors.text, fontSize: 26, fontWeight: "700" },
  subtitle: { color: theme.colors.textMuted, fontSize: 15, lineHeight: 22 },
  cardTitle: {
    color: theme.colors.text,
    fontSize: 15,
    fontWeight: "600",
    marginBottom: theme.spacing(1.5),
  },
  bankList: { flexDirection: "row", flexWrap: "wrap", gap: theme.spacing(1) },
  bankChip: {
    backgroundColor: theme.colors.surfaceAlt,
    borderRadius: theme.radius.sm,
    paddingHorizontal: theme.spacing(1.5),
    paddingVertical: theme.spacing(0.75),
  },
  bankText: { color: theme.colors.text, fontSize: 14 },
  note: {
    color: theme.colors.textMuted,
    fontSize: 13,
    marginTop: theme.spacing(1.5),
  },
});
