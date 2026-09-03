import React, { useCallback, useEffect, useState } from "react";
import { StyleSheet, Text, View } from "react-native";
import { SafeAreaView } from "react-native-safe-area-context";
import { api, ApiError } from "../api/client";
import { Button, ErrorView, LoadingView } from "../components";
import { theme } from "../theme";
import type { ScreenProps } from "../navigation/types";

/**
 * 3. ekrāns: "Analizējam tavus darījumus..."
 *
 * Divi soļi vienā ekrānā — vispirms darījumu sinhronizācija, tad AI analīze.
 * Kļūdas gadījumā lietotājs paliek šeit ar iespēju mēģināt vēlreiz vai
 * pāriet uz pārskatu ar jau esošajiem datiem.
 */
export function LoadingScreen({ navigation, route }: ScreenProps<"Loading">) {
  const [step, setStep] = useState(route.params.step);
  const [error, setError] = useState<string | null>(null);
  const [detail, setDetail] = useState<string | null>(null);

  const run = useCallback(async () => {
    setError(null);

    try {
      if (step === "sync") {
        const result = await api.syncTransactions();
        setDetail(`Ielādēti ${result.fetched} darījumi.`);
        setStep("analyze");
        return;
      }

      await api.analyze();
      navigation.replace("Dashboard");
    } catch (err) {
      setError(
        err instanceof ApiError
          ? err.message
          : "Neizdevās apstrādāt darījumus.",
      );
    }
  }, [step, navigation]);

  useEffect(() => {
    if (!error) {
      void run();
    }
    // `run` mainās līdz ar `step`, tāpēc nākamais solis sākas automātiski.
  }, [run, error]);

  if (error) {
    return (
      <SafeAreaView style={styles.safe}>
        <View style={styles.errorContent}>
          <Text style={styles.title}>Kaut kas nogāja greizi</Text>
          <ErrorView message={error} onRetry={run} />
          <Button
            title="Skatīt pārskatu bez analīzes"
            variant="secondary"
            onPress={() => navigation.replace("Dashboard")}
          />
        </View>
      </SafeAreaView>
    );
  }

  return (
    <SafeAreaView style={styles.safe}>
      <LoadingView
        label={
          step === "sync"
            ? "Ielādējam tavus darījumus no bankas..."
            : "Analizējam tavus darījumus...\nTas var aizņemt līdz minūtei."
        }
      />
      {detail ? <Text style={styles.detail}>{detail}</Text> : null}
    </SafeAreaView>
  );
}

const styles = StyleSheet.create({
  safe: { flex: 1, backgroundColor: theme.colors.background },
  errorContent: {
    flex: 1,
    justifyContent: "center",
    padding: theme.spacing(3),
    gap: theme.spacing(2),
  },
  title: { color: theme.colors.text, fontSize: 22, fontWeight: "700" },
  detail: {
    color: theme.colors.textMuted,
    fontSize: 14,
    textAlign: "center",
    paddingBottom: theme.spacing(4),
  },
});
