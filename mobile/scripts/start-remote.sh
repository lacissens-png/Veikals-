#!/usr/bin/env bash
#
# Palaiž Expo izstrādes serveri pret backend, kas darbojas internetā
# (piem. uz Render), nevis uz šī datora.
#
# Tā Expo Go ceļam vairs nevajag ne PostgreSQL, ne Docker, ne otru termināli —
# uz datora paliek tikai šī komanda, kas apkalpo JS bundli.
#
# Adresi var padot kā argumentu vai kā EXPO_PUBLIC_API_URL:
#   npm run start:remote -- https://tavs-serviss.onrender.com
#   EXPO_PUBLIC_API_URL=https://tavs-serviss.onrender.com npm run start:remote
#
# Pārējie argumenti tiek padoti tālāk `expo start`, piem. `-- <url> --web`.

set -euo pipefail

URL="${EXPO_PUBLIC_API_URL:-}"

# Ja pirmais arguments ir adrese, tā uzvar; atlikušie iet tālāk uz expo.
if [[ "${1:-}" == http://* || "${1:-}" == https://* ]]; then
  URL="$1"
  shift
fi

if [[ -z "${URL}" ]]; then
  cat >&2 <<'MSG'
KĻŪDA: nav norādīta backend adrese.

Šis skripts ir domāts izvietotam backend. Norādi adresi vienā no diviem veidiem:

  npm run start:remote -- https://tavs-serviss.onrender.com
  EXPO_PUBLIC_API_URL=https://tavs-serviss.onrender.com npm run start:remote

Ja backend darbojas uz šī paša datora, lieto `npm start` — tur adresi
nevajag, lietotne to atrod pati.
MSG
  exit 1
fi

if [[ ! "${URL}" =~ ^https?:// ]]; then
  echo "KĻŪDA: adresei jāsākas ar http:// vai https:// — saņemts: ${URL}" >&2
  exit 1
fi

# Trailing slash salauztu ceļus (`.../api` vs `...//api`), tāpēc nogriežam.
URL="${URL%/}"

# localhost uz telefona nozīmē pašu telefonu, ne šo datoru. Klusi tas izskatās
# pēc tīkla kļūdas, tāpēc pasakām uzreiz.
if [[ "${URL}" =~ ^https?://(localhost|127\.0\.0\.1)(:|$|/) ]]; then
  echo "KĻŪDA: ${URL} no telefona nav sasniedzams." >&2
  # Pēdiņas ap MSG ir svarīgas: bez tām atpakaļpēdiņas zemāk izpildītos kā komanda.
  cat >&2 <<'MSG'

Telefonam "localhost" ir pats telefons, ne šis dators. Lokālam backend lieto
`npm start` — lietotne datora adresi atrod pati.
MSG
  exit 1
fi

echo "Backend: ${URL}"
echo

export EXPO_PUBLIC_API_URL="${URL}"
exec npx expo start "$@"
