/** Vienota vizuālā valoda visiem ekrāniem. */
export const theme = {
  colors: {
    background: "#0f1115",
    surface: "#1a1d24",
    surfaceAlt: "#22262f",
    border: "#2c313c",
    text: "#f2f4f8",
    textMuted: "#9aa3b2",
    primary: "#4c8dff",
    primaryText: "#ffffff",
    danger: "#ff5f5f",
    warning: "#ffb020",
    success: "#3ecf8e",
  },
  spacing: (n: number) => n * 8,
  radius: { sm: 8, md: 12, lg: 16 },
} as const;

/** Kategoriju nosaukumi latviski (backend tos glabā angliski). */
export const CATEGORY_LABELS: Record<string, string> = {
  streaming: "Straumēšana",
  utilities: "Komunālie",
  insurance: "Apdrošināšana",
  telecom: "Telekomunikācijas",
  subscription_software: "Programmatūra",
  food_delivery: "Ēdiena piegāde",
  gym_fitness: "Sports",
  other: "Citi",
};

export function categoryLabel(category: string | null | undefined): string {
  if (!category) return CATEGORY_LABELS.other;
  return CATEGORY_LABELS[category] ?? category;
}

const FREQUENCY_LABELS: Record<string, string> = {
  monthly: "mēnesī",
  weekly: "nedēļā",
  yearly: "gadā",
};

export function frequencyLabel(frequency: string | null | undefined): string {
  if (!frequency) return "";
  return FREQUENCY_LABELS[frequency] ?? frequency;
}

export function formatEur(amount: number | null | undefined): string {
  if (amount === null || amount === undefined) return "—";
  return `${amount.toFixed(2)} €`;
}
