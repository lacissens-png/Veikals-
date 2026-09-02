import React from "react";
import { ActivityIndicator, Pressable, StyleSheet, Text, View } from "react-native";
import { StatusBar } from "expo-status-bar";
import { SafeAreaProvider } from "react-native-safe-area-context";
import {
  DarkTheme,
  NavigationContainer,
  type LinkingOptions,
} from "@react-navigation/native";
import { createNativeStackNavigator } from "@react-navigation/native-stack";
import { AuthProvider, useAuth } from "./src/auth/AuthContext";
import { BankConnectScreen } from "./src/screens/BankConnectScreen";
import { DashboardScreen } from "./src/screens/DashboardScreen";
import { DraftActionScreen } from "./src/screens/DraftActionScreen";
import { LoadingScreen } from "./src/screens/LoadingScreen";
import { OnboardingScreen } from "./src/screens/OnboardingScreen";
import { SettingsScreen } from "./src/screens/SettingsScreen";
import { SubscriptionDetailScreen } from "./src/screens/SubscriptionDetailScreen";
import { theme } from "./src/theme";
import type { RootStackParamList } from "./src/navigation/types";

const Stack = createNativeStackNavigator<RootStackParamList>();

/** Deep link, uz kuru banka atgriež lietotāju pēc autorizācijas. */
const linking: LinkingOptions<RootStackParamList> = {
  prefixes: ["abonementi://"],
  config: {
    screens: {
      Dashboard: "dashboard",
      BankConnect: "bank-callback",
    },
  },
};

const navigationTheme = {
  ...DarkTheme,
  colors: {
    ...DarkTheme.colors,
    background: theme.colors.background,
    card: theme.colors.surface,
    text: theme.colors.text,
    border: theme.colors.border,
    primary: theme.colors.primary,
  },
};

function RootNavigator() {
  const { initializing, token } = useAuth();

  if (initializing) {
    return (
      <View style={styles.splash}>
        <ActivityIndicator size="large" color={theme.colors.primary} />
      </View>
    );
  }

  return (
    <Stack.Navigator
      screenOptions={{
        headerStyle: { backgroundColor: theme.colors.surface },
        headerTitleStyle: { color: theme.colors.text },
        headerTintColor: theme.colors.text,
        contentStyle: { backgroundColor: theme.colors.background },
      }}
    >
      {token === null ? (
        <Stack.Screen
          name="Onboarding"
          component={OnboardingScreen}
          options={{ headerShown: false }}
        />
      ) : (
        <>
          <Stack.Screen
            name="Dashboard"
            component={DashboardScreen}
            options={({ navigation }) => ({
              title: "Pārskats",
              headerRight: () => (
                <Pressable
                  accessibilityRole="button"
                  accessibilityLabel="Iestatījumi"
                  onPress={() => navigation.navigate("Settings")}
                  hitSlop={12}
                >
                  <Text style={styles.headerAction}>Iestatījumi</Text>
                </Pressable>
              ),
            })}
          />
          <Stack.Screen
            name="BankConnect"
            component={BankConnectScreen}
            options={{ title: "Bankas savienojums" }}
          />
          <Stack.Screen
            name="Loading"
            component={LoadingScreen}
            options={{ headerShown: false, gestureEnabled: false }}
          />
          <Stack.Screen
            name="SubscriptionDetail"
            component={SubscriptionDetailScreen}
            options={({ route }) => ({ title: route.params.merchantName })}
          />
          <Stack.Screen
            name="DraftAction"
            component={DraftActionScreen}
            options={{ title: "Melnraksts" }}
          />
          <Stack.Screen
            name="Settings"
            component={SettingsScreen}
            options={{ title: "Iestatījumi" }}
          />
        </>
      )}
    </Stack.Navigator>
  );
}

export default function App() {
  return (
    <SafeAreaProvider>
      <AuthProvider>
        <NavigationContainer theme={navigationTheme} linking={linking}>
          <StatusBar style="light" />
          <RootNavigator />
        </NavigationContainer>
      </AuthProvider>
    </SafeAreaProvider>
  );
}

const styles = StyleSheet.create({
  splash: {
    flex: 1,
    alignItems: "center",
    justifyContent: "center",
    backgroundColor: theme.colors.background,
  },
  headerAction: { color: theme.colors.primary, fontSize: 15 },
});
