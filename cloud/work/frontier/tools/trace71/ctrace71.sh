#!/bin/sh
# usage: SCR=.. ctrace71.sh SRC FN   -- 7.1 colouring trace of FN (reduced TU of SRC, LOCK flags + -Wab,-r4300_mul).
# One sum71.sh line per decision, variable webs named from a -g3 build (vars71.sh); raw [CDX] records saved to
# $LOUT/<FN>.<SRC basename>.cdx (+ .vars); RAW=1 prints them.
# Extra uopt env via XENV="K=V ...".
. "$(dirname "$0")/lib/env71.sh"
src=$1; n=$2; b=$(push "$src") || exit 1
out="$LOUT/$n.$(basename "$src" .c).cdx"
rtk cdx $b $n $XENV > "$out" || { cat "$out"; exit 1; }
rtk vars $b $n 2>/dev/null | grep -E '^-?[0-9]+ [A-Za-z_]' > "$out.vars"
echo "$n: $(grep -m1 -o 'procname proc=[0-9]*' "$out" | sed 's/.*=/ordinal /'), $(grep -c 'p1dec' "$out") p1 + $(grep -c 'p2dec' "$out") p2 decisions; raw $out"
if [ -n "$RAW" ]; then cat "$out"; else sh "$TK/sum71.sh" "$out" "$out.vars"; fi
