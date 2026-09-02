import React, {
  createContext,
  useCallback,
  useContext,
  useEffect,
  useMemo,
  useState,
} from "react";
import * as SecureStore from "expo-secure-store";
import { api, setAuthToken } from "../api/client";
import type { AuthUser } from "../api/types";

const TOKEN_KEY = "abonementi.token";
const EMAIL_KEY = "abonementi.email";

interface AuthState {
  /** true, kamēr tiek lasīts saglabātais token (app startā). */
  initializing: boolean;
  token: string | null;
  user: Pick<AuthUser, "id" | "email"> | null;
  signIn: (email: string, password: string) => Promise<void>;
  signUp: (email: string, password: string) => Promise<void>;
  signOut: () => Promise<void>;
}

const AuthContext = createContext<AuthState | null>(null);

export function AuthProvider({ children }: { children: React.ReactNode }) {
  const [initializing, setInitializing] = useState(true);
  const [token, setToken] = useState<string | null>(null);
  const [user, setUser] = useState<Pick<AuthUser, "id" | "email"> | null>(null);

  // Sesijas atjaunošana no ierīces droša krātuves.
  useEffect(() => {
    let cancelled = false;

    (async () => {
      try {
        const [storedToken, storedEmail] = await Promise.all([
          SecureStore.getItemAsync(TOKEN_KEY),
          SecureStore.getItemAsync(EMAIL_KEY),
        ]);

        if (cancelled) return;

        if (storedToken) {
          setAuthToken(storedToken);
          setToken(storedToken);
          setUser(storedEmail ? { id: "", email: storedEmail } : null);
        }
      } finally {
        if (!cancelled) setInitializing(false);
      }
    })();

    return () => {
      cancelled = true;
    };
  }, []);

  const persist = useCallback(
    async (nextToken: string, nextUser: Pick<AuthUser, "id" | "email">) => {
      setAuthToken(nextToken);
      setToken(nextToken);
      setUser(nextUser);
      await Promise.all([
        SecureStore.setItemAsync(TOKEN_KEY, nextToken),
        SecureStore.setItemAsync(EMAIL_KEY, nextUser.email),
      ]);
    },
    [],
  );

  const signIn = useCallback(
    async (email: string, password: string) => {
      const result = await api.login(email, password);
      await persist(result.token, result.user);
    },
    [persist],
  );

  const signUp = useCallback(
    async (email: string, password: string) => {
      const result = await api.register(email, password);
      await persist(result.token, result.user);
    },
    [persist],
  );

  const signOut = useCallback(async () => {
    setAuthToken(null);
    setToken(null);
    setUser(null);
    await Promise.all([
      SecureStore.deleteItemAsync(TOKEN_KEY),
      SecureStore.deleteItemAsync(EMAIL_KEY),
    ]);
  }, []);

  const value = useMemo<AuthState>(
    () => ({ initializing, token, user, signIn, signUp, signOut }),
    [initializing, token, user, signIn, signUp, signOut],
  );

  return <AuthContext.Provider value={value}>{children}</AuthContext.Provider>;
}

export function useAuth(): AuthState {
  const context = useContext(AuthContext);
  if (!context) {
    throw new Error("useAuth jāizmanto AuthProvider iekšienē");
  }
  return context;
}
