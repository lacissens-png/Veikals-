import React, { useState } from "react";
import {
  KeyboardAvoidingView,
  Platform,
  ScrollView,
  StyleSheet,
  Text,
  TextInput,
  View,
} from "react-native";
import { SafeAreaView } from "react-native-safe-area-context";
import { ApiError } from "../api/client";
import { useAuth } from "../auth/AuthContext";
import { Button, ErrorView } from "../components";
import { theme } from "../theme";

/**
 * 1. ekrāns: laipni lūdzam + drošības skaidrojums + konta izveide.
 * Pieteikšanās forma parādās pēc "Sākt" pogas, lai pirmais skats paliek mierīgs.
 */
export function OnboardingScreen() {
  const { signIn, signUp } = useAuth();
  const [showForm, setShowForm] = useState(false);
  const [mode, setMode] = useState<"register" | "login">("register");
  const [email, setEmail] = useState("");
  const [password, setPassword] = useState("");
  const [loading, setLoading] = useState(false);
  const [error, setError] = useState<string | null>(null);

  async function submit() {
    setError(null);

    if (!email.includes("@")) {
      setError("Ievadi derīgu e-pasta adresi.");
      return;
    }
    if (password.length < 8) {
      setError("Parolei jābūt vismaz 8 rakstzīmes garai.");
      return;
    }

    setLoading(true);
    try {
      if (mode === "register") {
        await signUp(email, password);
      } else {
        await signIn(email, password);
      }
      // Navigācija notiek automātiski — App.tsx maina steku pēc token.
    } catch (err) {
      setError(
        err instanceof ApiError ? err.message : "Neizdevās pieteikties.",
      );
    } finally {
      setLoading(false);
    }
  }

  return (
    <SafeAreaView style={styles.safe} edges={["top", "bottom"]}>
      <KeyboardAvoidingView
        style={styles.flex}
        behavior={Platform.OS === "ios" ? "padding" : undefined}
      >
        <ScrollView contentContainerStyle={styles.content}>
          <Text style={styles.title}>Abonementu audits</Text>
          <Text style={styles.subtitle}>
            Savieno bankas kontu, un mēs atradīsim visus tavus abonementus un
            regulāros rēķinus — arī tos, par kuriem esi aizmirsis.
          </Text>

          <View style={styles.securityBox}>
            <Text style={styles.securityTitle}>Kā mēs sargājam tavus datus</Text>
            <Text style={styles.securityItem}>
              • Tikai lasīšanas piekļuve. Mēs nevaram veikt maksājumus vai
              pārskaitījumus.
            </Text>
            <Text style={styles.securityItem}>
              • Savienojums notiek caur licencētu PSD2 pakalpojuma sniedzēju —
              tavus bankas datus mēs neredzam.
            </Text>
            <Text style={styles.securityItem}>
              • Analīzei uz AI tiek sūtīts tikai datums, apraksts un summa. Tavs
              vārds, e-pasts un konta numurs nekad neatstāj mūsu serveri.
            </Text>
            <Text style={styles.securityItem}>
              • Atcelšanas vēstules mēs sagatavojam, bet nekad nesūtām bez tavas
              ziņas — tu tās nokopē un nosūti pats.
            </Text>
            <Text style={styles.securityItem}>
              • Kontu un visus datus vari izdzēst jebkurā brīdī iestatījumos.
            </Text>
          </View>

          {showForm ? (
            <View style={styles.form}>
              <Text style={styles.label}>E-pasts</Text>
              <TextInput
                style={styles.input}
                value={email}
                onChangeText={setEmail}
                autoCapitalize="none"
                autoCorrect={false}
                keyboardType="email-address"
                textContentType="emailAddress"
                placeholder="tavs@epasts.lv"
                placeholderTextColor={theme.colors.textMuted}
                editable={!loading}
              />

              <Text style={styles.label}>Parole</Text>
              <TextInput
                style={styles.input}
                value={password}
                onChangeText={setPassword}
                secureTextEntry
                textContentType="password"
                placeholder="vismaz 8 rakstzīmes"
                placeholderTextColor={theme.colors.textMuted}
                editable={!loading}
              />

              {error ? (
                <View style={styles.errorWrap}>
                  <ErrorView message={error} />
                </View>
              ) : null}

              <Button
                title={mode === "register" ? "Izveidot kontu" : "Pieteikties"}
                onPress={submit}
                loading={loading}
              />

              <Button
                title={
                  mode === "register"
                    ? "Man jau ir konts"
                    : "Izveidot jaunu kontu"
                }
                variant="secondary"
                disabled={loading}
                onPress={() => {
                  setMode(mode === "register" ? "login" : "register");
                  setError(null);
                }}
              />
            </View>
          ) : (
            <View style={styles.form}>
              <Button title="Sākt" onPress={() => setShowForm(true)} />
            </View>
          )}
        </ScrollView>
      </KeyboardAvoidingView>
    </SafeAreaView>
  );
}

const styles = StyleSheet.create({
  safe: { flex: 1, backgroundColor: theme.colors.background },
  flex: { flex: 1 },
  content: { padding: theme.spacing(3), gap: theme.spacing(2) },
  title: { color: theme.colors.text, fontSize: 32, fontWeight: "700" },
  subtitle: {
    color: theme.colors.textMuted,
    fontSize: 16,
    lineHeight: 24,
  },
  securityBox: {
    backgroundColor: theme.colors.surface,
    borderRadius: theme.radius.lg,
    borderWidth: 1,
    borderColor: theme.colors.border,
    padding: theme.spacing(2),
    gap: theme.spacing(1),
  },
  securityTitle: {
    color: theme.colors.text,
    fontSize: 16,
    fontWeight: "600",
    marginBottom: theme.spacing(0.5),
  },
  securityItem: {
    color: theme.colors.textMuted,
    fontSize: 14,
    lineHeight: 21,
  },
  form: { gap: theme.spacing(1.5), marginTop: theme.spacing(1) },
  label: { color: theme.colors.textMuted, fontSize: 13 },
  input: {
    backgroundColor: theme.colors.surfaceAlt,
    borderWidth: 1,
    borderColor: theme.colors.border,
    borderRadius: theme.radius.md,
    paddingHorizontal: theme.spacing(2),
    paddingVertical: 14,
    color: theme.colors.text,
    fontSize: 16,
  },
  errorWrap: { marginVertical: theme.spacing(0.5) },
});
