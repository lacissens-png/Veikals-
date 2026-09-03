import React, { useCallback, useEffect, useState } from "react";
import { ScrollView, StyleSheet, Text, View } from "react-native";
import { SafeAreaView } from "react-native-safe-area-context";
import * as Clipboard from "expo-clipboard";
import { api, ApiError } from "../api/client";
import { Button, Card, ErrorView, LoadingView } from "../components";
import { theme } from "../theme";
import type { ScreenProps } from "../navigation/types";

/**
 * 6. ekrāns: sagatavotais e-pasta teksts ar "Kopēt" pogu.
 *
 * MVP apzināti NENOSŪTA vēstuli — lietotājs to nokopē un nosūta pats.
 */
export function DraftActionScreen({
  navigation,
  route,
}: ScreenProps<"DraftAction">) {
  const { subscriptionId, merchantName, actionType } = route.params;

  const [draftId, setDraftId] = useState<string | null>(null);
  const [text, setText] = useState<string | null>(null);
  const [loading, setLoading] = useState(true);
  const [error, setError] = useState<string | null>(null);
  const [copied, setCopied] = useState(false);
  const [sending, setSending] = useState(false);
  const [sent, setSent] = useState<{ recipient: string | null; simulated: boolean } | null>(null);

  const generate = useCallback(async () => {
    setLoading(true);
    setError(null);
    setCopied(false);

    try {
      const draft = await api.createDraft(subscriptionId, actionType);
      setDraftId(draft.id);
      setText(draft.draftText);
    } catch (err) {
      setError(
        err instanceof ApiError
          ? err.message
          : "Neizdevās sagatavot melnrakstu.",
      );
    } finally {
      setLoading(false);
    }
  }, [subscriptionId, actionType]);

  useEffect(() => {
    void generate();
  }, [generate]);

  async function send() {
    if (!draftId) return;
    setSending(true);
    setError(null);
    try {
      const result = await api.sendDraft(draftId);
      setSent({ recipient: result.recipient, simulated: result.simulated });
    } catch (err) {
      setError(
        err instanceof ApiError ? err.message : "Neizdevās nosūtīt vēstuli.",
      );
    } finally {
      setSending(false);
    }
  }

  async function copy() {
    if (!text) return;

    await Clipboard.setStringAsync(text);
    setCopied(true);

    // Statuss ir informatīvs — ja tā saglabāšana neizdodas, kopēšana tik un tā
    // ir notikusi, tāpēc kļūdu tikai norijam.
    if (draftId) {
      try {
        await api.markDraft(draftId, "copied");
      } catch {
        /* neietekmē lietotāju */
      }
    }
  }

  if (loading) {
    return (
      <SafeAreaView style={styles.safe} edges={["bottom"]}>
        <LoadingView label="Rakstām vēstuli..." />
      </SafeAreaView>
    );
  }

  return (
    <SafeAreaView style={styles.safe} edges={["bottom"]}>
      <ScrollView contentContainerStyle={styles.content}>
        <Text style={styles.title}>
          {actionType === "cancel" ? "Atcelšanas vēstule" : "Pārrunu vēstule"}
        </Text>
        <Text style={styles.subtitle}>
          Adresāts: {merchantName}. Pārlasi tekstu un vai nu nosūti to no sava
          pasta, vai nokopē un nosūti pats. Bez tavas komandas mēs neko nesūtām.
        </Text>

        {error ? <ErrorView message={error} onRetry={generate} /> : null}

        {text ? (
          <Card>
            <Text style={styles.draftText} selectable>
              {text}
            </Text>
          </Card>
        ) : null}

        {sent ? (
          <View style={styles.copiedBox}>
            <Text style={styles.copiedText}>
              {sent.simulated
                ? `Mock režīms: vēstule NETIKA nosūtīta. Īstā adresāts būtu ${sent.recipient}.`
                : `Nosūtīts uz ${sent.recipient}.`}
            </Text>
          </View>
        ) : null}

        {copied ? (
          <View style={styles.copiedBox}>
            <Text style={styles.copiedText}>
              Nokopēts starpliktuvē. Ielīmē to sava e-pasta klientā.
            </Text>
          </View>
        ) : null}

        <View style={styles.actions}>
          <Button
            title={sent ? "Nosūtīts" : "Nosūtīt no mana pasta"}
            onPress={() => void send()}
            loading={sending}
            disabled={!text || sent !== null}
          />
          <Button
            title={copied ? "Kopēt vēlreiz" : "Kopēt tekstu"}
            variant="secondary"
            onPress={() => void copy()}
            disabled={!text}
          />
          <Button
            title="Ģenerēt citu variantu"
            variant="secondary"
            onPress={generate}
          />
          <Button
            title="Atpakaļ"
            variant="secondary"
            onPress={() => navigation.goBack()}
          />
        </View>
      </ScrollView>
    </SafeAreaView>
  );
}

const styles = StyleSheet.create({
  safe: { flex: 1, backgroundColor: theme.colors.background },
  content: { padding: theme.spacing(2), gap: theme.spacing(2) },
  title: { color: theme.colors.text, fontSize: 22, fontWeight: "700" },
  subtitle: { color: theme.colors.textMuted, fontSize: 14, lineHeight: 21 },
  draftText: { color: theme.colors.text, fontSize: 15, lineHeight: 23 },
  copiedBox: {
    backgroundColor: "rgba(62, 207, 142, 0.12)",
    borderColor: theme.colors.success,
    borderWidth: 1,
    borderRadius: theme.radius.md,
    padding: theme.spacing(1.5),
  },
  copiedText: { color: theme.colors.success, fontSize: 14 },
  actions: { gap: theme.spacing(1) },
});
