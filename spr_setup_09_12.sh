#!/usr/bin/env bash
set -euo pipefail

# -----------------------------------------------------------------------------
# SPR downloader – Problems 09–12 (MANUÁLNÍ seznam, bez parsování HTML)
#
# Vytvoří:
#   problems-09, problems-10, problems-11, problems-12
# a v každém:
#   "<code> - <title>/<code>.pdf"
#
# PDF na serveru jsou jen "<cislo>.pdf":
#   https://www.cs.vsb.cz/sawa/spr/zadani/10154.pdf
#
# Spuštění:
#   chmod +x spr_setup_09_12_manual.sh
#   ./spr_setup_09_12_manual.sh
#
# SSL problémy:
#   WGET_OPTS="--no-check-certificate" ./spr_setup_09_12_manual.sh
#
# Změna cílové složky:
#   BASE_DIR="/cesta/k/spr" ./spr_setup_09_12_manual.sh
# -----------------------------------------------------------------------------

BASE_DIR="${BASE_DIR:-/home/xpolas/banska/spr}"
ZADANI_BASE="https://www.cs.vsb.cz/sawa/spr/zadani"
WGET_OPTS="${WGET_OPTS:-}"

cd "$BASE_DIR" || { echo "Chyba: BASE_DIR neexistuje: $BASE_DIR" >&2; exit 1; }

sanitize_dirname() {
  # bezpečný název složky pro Linux FS
  echo "$1" | sed -E 's/[\/\\:*?"<>|]/-/g; s/[[:space:]]+/ /g; s/[[:space:]]+$//g'
}

download_one() {
  local set="$1" code="$2" title="$3"
  local set_dir="problems-$(printf '%02d' "$set")"

  local safe_title
  safe_title="$(sanitize_dirname "$title")"

  local task_dir="${set_dir}/${code} - ${safe_title}"
  local url="${ZADANI_BASE}/${code}.pdf"
  local dest="${task_dir}/${code}.pdf"

  mkdir -p "$task_dir"

  if [[ -s "$dest" ]]; then
    echo "OK: $dest (už existuje)"
    return 0
  fi

  echo "Stahuju $url -> $dest"
  wget $WGET_OPTS -q --show-progress -O "$dest" "$url"
}

# ----------------------- SEZNAM ÚLOH (09–12) -----------------------
# Formát: set<TAB>code<TAB>title
cat <<'LIST' | while IFS=$'\t' read -r set code title; do
9	10054	The Necklace
9	10092	The Problem with the Problem Setter
9	10199	Tourist Guide

10	10157	Expressions
10	10158	War
10	10632	Pyramid

11	10001	Garden of Eden
11	10065	Useless Tile Packers
11	10088	Trees on My Island
11	10090	Marbles
11	10136	Chocolate Chip Cookies
11	10181	15-Puzzle Problem
11	10247	Complete Tree Labeling

12	116	Unidirectional TSP
12	310	L-system
12	312	Crosswords (II)
12	337	Interpreting Control Sequences
12	657	The die is cast
12	706	LC-Display
12	707	Robbery
12	796	Critical Links
12	848	Fmt
12	887	Revolutionary Calendar
12	10029	Edit Step Ladders
12	10033	Interpreter
12	10043	Chainsaw Massacre
12	10051	Tower of Cubes
12	10075	Airlines
12	10149	Yahtzee
12	10150	Doublets
12	10154	Weights and Measures
12	10159	Star
12	10202	Pairsumonious Numbers
12	10315	Poker Hands
12	10759	Dice Throwing
12	10839	The Curse of Barbosa
LIST
  # přeskoč prázdné řádky / oddělovače
  [[ -z "${set// }" ]] && continue
  [[ -z "${code// }" ]] && continue
  [[ -z "${title// }" ]] && continue

  download_one "$set" "$code" "$title"
done

echo ""
echo "Hotovo. Sety: problems-09 .. problems-12"
