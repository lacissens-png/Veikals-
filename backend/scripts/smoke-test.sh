#!/usr/bin/env bash
#
# Dūmu tests: startē uzbūvēto serveri un izbrauc visu plūsmu mock režīmā.
#
# Pārbauda to, ko tipu pārbaude nevar: ka serveris tiešām startē, savienojas ar
# datubāzi, apkalpo autentifikāciju un ka atkārtota sinhronizācija nedublē
# darījumus.
#
# Priekšnosacījumi: `npm run build`, palaistas migrācijas, un vidē iestatīts
# DATABASE_URL un JWT_SECRET.
#
# Lietošana:  ./scripts/smoke-test.sh
#
# AI endpointi šeit netiek skarti — tiem vajadzīga ANTHROPIC_API_KEY.

set -euo pipefail

PORT="${SMOKE_PORT:-4123}"
BASE="http://127.0.0.1:${PORT}"
API="${BASE}/api"
EMAIL="smoke-$(date +%s)-$$@example.com"
PASSWORD="smoke-parole-12345"
SERVER_LOG="$(mktemp)"
SERVER_PID=""

cleanup() {
  if [[ -n "${SERVER_PID}" ]] && kill -0 "${SERVER_PID}" 2>/dev/null; then
    kill "${SERVER_PID}" 2>/dev/null || true
    wait "${SERVER_PID}" 2>/dev/null || true
  fi
}
trap cleanup EXIT

fail() {
  echo "DŪMU TESTS NEIZDEVĀS: $*" >&2
  echo "--- servera žurnāls ---" >&2
  cat "${SERVER_LOG}" >&2
  exit 1
}

# Izvelk lauku no JSON, izmantojot node (tas jau ir pieejams).
json_field() {
  node -e '
    let raw = "";
    process.stdin.on("data", (chunk) => (raw += chunk));
    process.stdin.on("end", () => {
      try {
        const value = process.argv[1]
          .split(".")
          .reduce((acc, key) => acc?.[key], JSON.parse(raw));
        if (value === undefined || value === null) process.exit(1);
        process.stdout.write(String(value));
      } catch {
        process.exit(1);
      }
    });
  ' "$1"
}

echo "==> Startējam serveri portā ${PORT}"
PORT="${PORT}" \
  ENABLE_BANKING_MOCK=true \
  ENABLE_BANKING_REDIRECT_URI="${BASE}/api/bank/callback" \
  node dist/server.js > "${SERVER_LOG}" 2>&1 &
SERVER_PID=$!

for _ in $(seq 30); do
  if curl -fsS -m 2 "${BASE}/health" > /dev/null 2>&1; then break; fi
  kill -0 "${SERVER_PID}" 2>/dev/null || fail "serveris apstājās startējot"
  sleep 1
done

curl -fsS -m 5 "${BASE}/health" > /dev/null || fail "/health neatbild"
echo "    /health atbild"

echo "==> Reģistrācija"
TOKEN="$(curl -fsS -X POST "${API}/auth/register" \
  -H 'Content-Type: application/json' \
  -d "{\"email\":\"${EMAIL}\",\"password\":\"${PASSWORD}\"}" \
  | json_field token)" || fail "reģistrācija neizdevās"
AUTH="Authorization: Bearer ${TOKEN}"

echo "==> Neautentificēts pieprasījums atgriež 401"
STATUS="$(curl -s -o /dev/null -w '%{http_code}' "${API}/subscriptions")"
[[ "${STATUS}" == "401" ]] || fail "gaidījām 401 bez tokena, saņēmām ${STATUS}"

echo "==> Bankas savienojums (mock)"
AUTH_URL="$(curl -fsS -X POST "${API}/bank/connect" -H "${AUTH}" \
  | json_field authorizationUrl)" || fail "bank/connect neizdevās"

# Mock režīmā autorizācijas saite ved atpakaļ uz mūsu pašu callback.
curl -fsS -o /dev/null "${AUTH_URL}" || fail "bankas callback neizdevās"

echo "==> Pirmā sinhronizācija"
SYNC_ONE="$(curl -fsS -X POST "${API}/transactions/sync" -H "${AUTH}")" \
  || fail "transactions/sync neizdevās"
FETCHED="$(printf '%s' "${SYNC_ONE}" | json_field fetched)"
INSERTED="$(printf '%s' "${SYNC_ONE}" | json_field inserted)"

[[ "${FETCHED}" -gt 0 ]] || fail "banka neatgrieza nevienu darījumu"
[[ "${INSERTED}" -eq "${FETCHED}" ]] \
  || fail "gaidījām ${FETCHED} ievietotus, saņēmām ${INSERTED}"
echo "    ielādēti ${FETCHED}, ievietoti ${INSERTED}"

echo "==> Atkārtota sinhronizācija nedrīkst dublēt"
SYNC_TWO="$(curl -fsS -X POST "${API}/transactions/sync" -H "${AUTH}")" \
  || fail "atkārtota sinhronizācija neizdevās"
INSERTED_TWO="$(printf '%s' "${SYNC_TWO}" | json_field inserted)"

[[ "${INSERTED_TWO}" -eq 0 ]] \
  || fail "dedublikācija nenostrādāja — ievietoti vēl ${INSERTED_TWO}"
echo "    ievietoti 0 (dedublikācija strādā)"

echo "==> Abonementu saraksts atbild"
curl -fsS "${API}/subscriptions" -H "${AUTH}" | json_field summary.count > /dev/null \
  || fail "/subscriptions neatgrieza kopsavilkumu"

echo "==> Nezināms abonements atgriež 404"
STATUS="$(curl -s -o /dev/null -w '%{http_code}' \
  "${API}/subscriptions/00000000-0000-0000-0000-000000000000" -H "${AUTH}")"
[[ "${STATUS}" == "404" ]] || fail "gaidījām 404, saņēmām ${STATUS}"

echo
echo "Dūmu tests izgāja cauri."
