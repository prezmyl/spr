#!/usr/bin/env bash
set -euo pipefail

BASE_DIR="/home/xpolas/banska/spr"

# sem si případně můžeš dát "--no-check-certificate", pokud bude dělat problémy SSL
WGET_OPTS="--no-check-certificate"

cd "$BASE_DIR" || { echo "Adresář $BASE_DIR neexistuje"; exit 1; }

# Hlavní adresáře
mkdir -p problems07 problems08

# Problémy 7
codes7=(321 10044 10047)
names7=(
  "321 - The New Villa"
  "10044 - Erdös Numbers"
  "10047 - The Monocycle"
)

# Problémy 8
codes8=(704 705 10278)
names8=(
  "704 - Colour Hash"
  "705 - Slash Maze"
  "10278 - Fire Station"
)

download_set() {
  local parent="$1"
  shift
  local -n codes="$1"
  local -n names="$2"

  for i in "${!codes[@]}"; do
    code="${codes[$i]}"
    name="${names[$i]}"

    dir="$parent/$name"
    mkdir -p "$dir"

    url="https://www.cs.vsb.cz/sawa/spr/zadani/${code}.pdf"
    dest="$dir/${code}.pdf"

    echo "Stahuju $url -> $dest"
    wget $WGET_OPTS -q --show-progress -O "$dest" "$url"
  done
}

download_set "problems07" codes7 names7
download_set "problems08" codes8 names8

echo "Hotovo."

