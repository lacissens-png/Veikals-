#!/usr/bin/env bash
#
# Sagatavo projektu palaišanai: datubāze, atkarības, migrācijas.
# Palaid vienreiz pēc klonēšanas.
#
#   ./scripts/setup.sh

set -euo pipefail
cd "$(dirname "$0")/.."

step() { printf "\n\033[1m==> %s\033[0m\n" "$1"; }

step "Datubāze"
# Pārbaudām dēmonu, ne tikai bināro failu: `docker` var būt uzstādīts, bet
# Docker Desktop nepalaists — tad `docker compose` krīt.
if command -v docker >/dev/null 2>&1 && docker info >/dev/null 2>&1; then
  docker compose up -d
  printf "    gaidām, kamēr datubāze ir gatava"
  ready=""
  for _ in $(seq 30); do
    if docker compose exec -T db pg_isready -U postgres -d abonementi >/dev/null 2>&1; then
      ready="jā"; printf " gatava\n"; break
    fi
    printf "."; sleep 1
  done
  [ -n "$ready" ] || { printf "\n"; echo "    Datubāze neatbild. Skaties: docker compose logs db"; exit 1; }
elif pg_isready -h localhost -p 5432 >/dev/null 2>&1; then
  echo "    Docker nav pieejams, bet PostgreSQL uz 5432 atbild — lietojam to."
  createdb -h localhost -U postgres abonementi 2>/dev/null \
    && echo "    izveidota datubāze 'abonementi'" \
    || echo "    datubāze 'abonementi' jau eksistē"
else
  echo "    Nav ne Docker dēmona, ne PostgreSQL uz porta 5432."
  echo ""
  echo "    Palaid vienu no šiem un mēģini vēlreiz:"
  echo "      • Docker Desktop, tad: docker compose up -d"
  echo "      • vai vietējo PostgreSQL, tad: createdb abonementi"
  exit 1
fi

step "Backend konfigurācija"
if [ ! -f backend/.env ]; then
  cp backend/.env.example backend/.env
  echo "    izveidots backend/.env no parauga"
  if command -v openssl >/dev/null 2>&1; then
    key=$(openssl rand -hex 32)
    secret=$(openssl rand -hex 32)
    # Nomainām paraugu atslēgas ar īstām nejaušām.
    sed -i.bak "s|^TOKEN_ENCRYPTION_KEY=.*|TOKEN_ENCRYPTION_KEY=$key|" backend/.env
    sed -i.bak "s|^JWT_SECRET=.*|JWT_SECRET=$secret|" backend/.env
    rm -f backend/.env.bak
    echo "    ģenerētas nejaušas JWT_SECRET un TOKEN_ENCRYPTION_KEY vērtības"
  fi
else
  echo "    backend/.env jau eksistē — neaiztiekam"
fi

step "Backend atkarības un migrācijas"
(cd backend && npm install && npx prisma migrate deploy)

step "Lietotnes atkarības"
(cd mobile && npm install)

cat <<'DONE'

Gatavs. Palaišanai:

  ./scripts/start.sh

Tas palaiž backend un Expo, un Ctrl+C apstādina abus. Tad skenē QR kodu ar
Expo Go — lietotne pati atradīs backend uz tā paša datora, IP nav jāmeklē.

AI funkcijām ieliec ANTHROPIC_API_KEY failā backend/.env.
DONE
