#!/usr/bin/env bash
#
# Palaiž visu ar vienu komandu: datubāze, backend, Expo.
#
#   ./scripts/start.sh                    # viss uz šī datora
#   ./scripts/start.sh --web              # tas pats + atver pārlūkā
#   ./scripts/start.sh --remote <adrese>  # tikai Expo pret izvietotu backend
#
# Ctrl+C apstādina visu, arī backend, kas darbojas fonā.

set -euo pipefail
cd "$(dirname "$0")/.."

PORT=4000
LOG=backend/dev.log
SKIP_SETUP=""
REMOTE=""
EXPO_ARGS=()

while [ $# -gt 0 ]; do
  case "$1" in
    --remote)
      REMOTE="${2:-}"
      [ -n "$REMOTE" ] || { echo "KĻŪDA: --remote prasa adresi." >&2; exit 1; }
      shift 2
      ;;
    --skip-setup) SKIP_SETUP="jā"; shift ;;
    -h|--help) sed -n '2,10p' "$0" | sed 's/^# \?//'; exit 0 ;;
    *) EXPO_ARGS+=("$1"); shift ;;
  esac
done

step() { printf "\n\033[1m==> %s\033[0m\n" "$1"; }

# Izvietots backend: datubāze un serveris nav mūsu ziņā, tāpēc viss pārējais
# atkrīt. Adreses validācija jau ir start-remote.sh — nedublējam.
if [ -n "$REMOTE" ]; then
  exec mobile/scripts/start-remote.sh "$REMOTE" ${EXPO_ARGS[@]+"${EXPO_ARGS[@]}"}
fi

BACKEND_PID=""

# `npm run dev` startē `tsx watch`, kas savukārt startē node. Nogalinot tikai
# npm procesu, mazbērni paliek dzīvi un tur portu 4000 — nākamā palaišana tad
# krīt ar EADDRINUSE. Tāpēc ejam pa koku no apakšas uz augšu.
kill_tree() {
  local pid="$1" child
  for child in $(pgrep -P "$pid" 2>/dev/null || true); do
    kill_tree "$child"
  done
  kill "$pid" 2>/dev/null || true
}

cleanup() {
  [ -n "$BACKEND_PID" ] || return 0
  printf "\nApstādinām backend..."
  kill_tree "$BACKEND_PID"

  # Gaidām, kamēr ports tiešām atbrīvojas. Bez tā nākamā palaišana var krist
  # sacīkstes dēļ, un tas izskatītos pēc nejaušas kļūdas.
  for _ in $(seq 20); do
    port_busy || { printf " gatavs\n"; return 0; }
    sleep 0.5
  done
  printf "\nBRĪDINĀJUMS: ports %s joprojām aizņemts.\n" "$PORT"
}

port_busy() {
  # Bez lsof/ss atkarības: ja /health atbild, kaut kas tur klausās.
  curl -sf -m 2 "http://127.0.0.1:${PORT}/health" >/dev/null 2>&1
}

trap cleanup EXIT INT TERM

if port_busy; then
  echo "KĻŪDA: ports ${PORT} jau aizņemts — backend laikam jau darbojas." >&2
  echo "Apstādini to un mēģini vēlreiz, vai lieto: ./scripts/start.sh --remote <adrese>" >&2
  exit 1
fi

if [ -z "$SKIP_SETUP" ]; then
  # setup.sh ir idempotents: otrajā reizē tas tikai ātri pārbauda.
  ./scripts/setup.sh
fi

step "Backend"
# Izvade iet uz failu, nevis termināli: citādi tā aizskalotu Expo QR kodu.
: > "$LOG"
(cd backend && npm run dev) >> "$LOG" 2>&1 &
BACKEND_PID=$!

printf "    gaidām, kamēr atbild http://localhost:%s" "$PORT"
health=""
for _ in $(seq 60); do
  if ! kill -0 "$BACKEND_PID" 2>/dev/null; then
    printf "\n\nBackend apstājās. %s beigas:\n\n" "$LOG"
    tail -20 "$LOG"
    exit 1
  fi
  health="$(curl -sf -m 2 "http://127.0.0.1:${PORT}/health" 2>/dev/null || true)"
  [ -n "$health" ] && { printf " gatavs\n"; break; }
  printf "."
  sleep 1
done

if [ -z "$health" ]; then
  printf "\n\nBackend neatbildēja 60 sekundēs. %s beigas:\n\n" "$LOG"
  tail -20 "$LOG"
  exit 1
fi

echo "    žurnāls: $LOG"

# Bez atslēgas sinhronizācija izdodas, bet analīze atgriež 503 un pārskats
# paliek tukšs. Tas izskatās pēc salauztas lietotnes, tāpēc pasakām uzreiz.
case "$health" in
  *'"aiConfigured":false'*)
    printf "\n\033[33mBRĪDINĀJUMS:\033[0m ANTHROPIC_API_KEY nav iestatīta.\n"
    echo "Darījumi ielādēsies, bet abonementu analīze atgriezīs 503 un pārskats"
    echo "paliks tukšs. Atslēgu ieliec failā backend/.env un palaid vēlreiz."
    ;;
esac

step "Lietotne"
echo "    skenē QR kodu ar Expo Go (vajag SDK 57 versiju)"
echo "    Ctrl+C apstādina visu"
echo

cd mobile && npm start -- ${EXPO_ARGS[@]+"${EXPO_ARGS[@]}"}
