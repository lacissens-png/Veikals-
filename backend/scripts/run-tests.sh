#!/usr/bin/env bash
#
# Palaiž testus un pārliecinās, ka tie tiešām izpildījās.
#
# `node --test` ar glob, kas neko neatrod, iziet ar kodu 0. Bez šīs pārbaudes
# CI paliktu zaļš arī tad, ja testu faili pārvietoti vai glob pārstāj
# saskaņoties — t.i. tieši tad, kad testu aizsardzība ir pazudusi.

set -euo pipefail

PATTERN="${1:-src/**/*.test.ts}"

# Fiksēta atslēga tikai testiem — crypto testi bez tās nevar strādāt.
# Šī nekad netiek lietota ārpus testiem.
export TOKEN_ENCRYPTION_KEY="${TOKEN_ENCRYPTION_KEY:-$(printf '0%.0s' {1..64})}"

output=""
status=0
output="$(node --import tsx --test "${PATTERN}" 2>&1)" || status=$?

echo "${output}"

if [[ "${status}" -ne 0 ]]; then
  exit "${status}"
fi

passed="$(grep -E '^# pass ' <<< "${output}" | awk '{print $3}' | head -1)"

if [[ -z "${passed}" || "${passed}" -eq 0 ]]; then
  echo >&2
  echo "KĻŪDA: neviens tests netika izpildīts (šablons: ${PATTERN})." >&2
  echo "Testu faili pārvietoti vai pārsaukti? Šablons vairs nesaskaņojas." >&2
  exit 1
fi

echo
echo "Izpildīti ${passed} testi."
