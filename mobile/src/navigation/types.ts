import type { NativeStackScreenProps } from "@react-navigation/native-stack";

/** Ekrānu saraksts un to parametri. */
export type RootStackParamList = {
  Onboarding: undefined;
  BankConnect: undefined;
  Loading: { step: "sync" | "analyze" };
  Dashboard: undefined;
  SubscriptionDetail: { subscriptionId: string; merchantName: string };
  DraftAction: {
    subscriptionId: string;
    merchantName: string;
    actionType: "cancel" | "negotiate";
  };
  Settings: undefined;
};

export type ScreenProps<T extends keyof RootStackParamList> =
  NativeStackScreenProps<RootStackParamList, T>;
