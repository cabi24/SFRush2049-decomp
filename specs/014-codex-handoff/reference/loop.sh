#!/bin/sh
# usage: loop.sh FUNCTION SOURCE.c [IDO flags]   (run on Rocky; WT = a private repo copy with tools/cloud/score.py)
# Compiles SOURCE.c with the toolkit IDO, scores it strictly, and runs the workbench diagnose against the target object.
set -u
FN="$1"; SRC="$(realpath "$2")"; FLAGS="${3:--g0 -O2 -mips2 -G 0 -non_shared}"
WB="$HOME/agents/wb"; TK="$HOME/rush2049/cache/toolkits/796ae99a5cb7922e335e3afd87008753c573c82b45d04e8cfbce4f513cdbfbf5"
WT="${WT:-$HOME/agents/A/wt}"
OBJ="$(mktemp -d)/cand.o"
echo "== strict score ($FLAGS)"; (cd "$WT" && python3 tools/cloud/score.py fn "$SRC" "$FN" --flags "$FLAGS" 2>&1 | tail -1)
"$TK/ido/cc" -c $FLAGS "$SRC" -o "$OBJ" >/dev/null 2>"$OBJ.err" || { echo "compile failed:"; head -5 "$OBJ.err"; exit 1; }
echo "== diagnose"
LD_LIBRARY_PATH="$TK/lib" PYTHONPATH="$WB/wb/src" python3 -m decomp_workbench diagnose "$WB/targets/$FN.o" "$OBJ" --function "$FN" --objdump "$TK/bin/objdump" ${DIAG_ARGS:-} 2>&1 | grep -vE "^\s+(evidence|capture|proved)" | sed -n '1,70p' | cut -c1-200
