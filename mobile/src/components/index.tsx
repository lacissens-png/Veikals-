import React from "react";
import {
  ActivityIndicator,
  Pressable,
  StyleSheet,
  Text,
  View,
  type ViewStyle,
} from "react-native";
import { theme } from "../theme";

/** Poga ar ielādes stāvokli — visi API zvani to izmanto. */
export function Button({
  title,
  onPress,
  loading = false,
  disabled = false,
  variant = "primary",
}: {
  title: string;
  onPress: () => void;
  loading?: boolean;
  disabled?: boolean;
  variant?: "primary" | "secondary" | "danger";
}) {
  const isDisabled = disabled || loading;

  return (
    <Pressable
      accessibilityRole="button"
      accessibilityState={{ disabled: isDisabled, busy: loading }}
      onPress={onPress}
      disabled={isDisabled}
      style={({ pressed }) => [
        styles.button,
        variant === "secondary" && styles.buttonSecondary,
        variant === "danger" && styles.buttonDanger,
        isDisabled && styles.buttonDisabled,
        pressed && !isDisabled && styles.buttonPressed,
      ]}
    >
      {loading ? (
        <ActivityIndicator color={theme.colors.primaryText} />
      ) : (
        <Text
          style={[
            styles.buttonText,
            variant === "secondary" && styles.buttonTextSecondary,
          ]}
        >
          {title}
        </Text>
      )}
    </Pressable>
  );
}

/** Kļūdas paziņojums ar iespēju mēģināt vēlreiz. */
export function ErrorView({
  message,
  onRetry,
}: {
  message: string;
  onRetry?: () => void;
}) {
  return (
    <View style={styles.errorBox}>
      <Text style={styles.errorText}>{message}</Text>
      {onRetry ? (
        <View style={styles.errorAction}>
          <Button title="Mēģināt vēlreiz" onPress={onRetry} variant="secondary" />
        </View>
      ) : null}
    </View>
  );
}

/** Centrēts ielādes indikators ar paskaidrojumu. */
export function LoadingView({ label }: { label: string }) {
  return (
    <View style={styles.loadingBox}>
      <ActivityIndicator size="large" color={theme.colors.primary} />
      <Text style={styles.loadingText}>{label}</Text>
    </View>
  );
}

export function Card({
  children,
  style,
}: {
  children: React.ReactNode;
  style?: ViewStyle;
}) {
  return <View style={[styles.card, style]}>{children}</View>;
}

export function Badge({
  label,
  tone = "neutral",
}: {
  label: string;
  tone?: "neutral" | "warning" | "danger" | "success";
}) {
  const toneColor = {
    neutral: theme.colors.textMuted,
    warning: theme.colors.warning,
    danger: theme.colors.danger,
    success: theme.colors.success,
  }[tone];

  return (
    <View style={[styles.badge, { borderColor: toneColor }]}>
      <Text style={[styles.badgeText, { color: toneColor }]}>{label}</Text>
    </View>
  );
}

const styles = StyleSheet.create({
  button: {
    backgroundColor: theme.colors.primary,
    paddingVertical: 14,
    paddingHorizontal: theme.spacing(2),
    borderRadius: theme.radius.md,
    alignItems: "center",
    justifyContent: "center",
    minHeight: 50,
  },
  buttonSecondary: {
    backgroundColor: "transparent",
    borderWidth: 1,
    borderColor: theme.colors.border,
  },
  buttonDanger: { backgroundColor: theme.colors.danger },
  buttonDisabled: { opacity: 0.5 },
  buttonPressed: { opacity: 0.85 },
  buttonText: {
    color: theme.colors.primaryText,
    fontSize: 16,
    fontWeight: "600",
  },
  buttonTextSecondary: { color: theme.colors.text },
  errorBox: {
    backgroundColor: "rgba(255, 95, 95, 0.12)",
    borderColor: theme.colors.danger,
    borderWidth: 1,
    borderRadius: theme.radius.md,
    padding: theme.spacing(2),
    gap: theme.spacing(1.5),
  },
  errorText: { color: theme.colors.text, fontSize: 14, lineHeight: 20 },
  errorAction: { marginTop: theme.spacing(0.5) },
  loadingBox: {
    flex: 1,
    alignItems: "center",
    justifyContent: "center",
    gap: theme.spacing(2),
    padding: theme.spacing(3),
  },
  loadingText: {
    color: theme.colors.textMuted,
    fontSize: 16,
    textAlign: "center",
  },
  card: {
    backgroundColor: theme.colors.surface,
    borderRadius: theme.radius.lg,
    borderWidth: 1,
    borderColor: theme.colors.border,
    padding: theme.spacing(2),
  },
  badge: {
    borderWidth: 1,
    borderRadius: theme.radius.sm,
    paddingHorizontal: theme.spacing(1),
    paddingVertical: 2,
    alignSelf: "flex-start",
  },
  badgeText: { fontSize: 12, fontWeight: "600" },
});
