#!/bin/sh
# usage: TAG=.. ctrace.sh NAME LABEL [CAND.c]  -- colouring trace of procedure NAME on snapshot st_LABEL
# (with CAND.c: unit-score NAME with it and snapshot first). Prints the globalcolor ordinal, then one sum.sh line per
# decision. Raw [CDX] records (p1cand/p1cost/p1dec/p1color/webdetail/interference ...) saved to $LOUT/LABEL.NAME.cdx.
# RAW=1 prints the raw records instead. Extra uopt env (e.g. CDX_FORCE needs force.sh, not this) via XENV="K=V ...".
. "$(dirname "$0")/lib/env.sh"
n=$1; lab=$2
if [ -n "$3" ]; then unit_score $n "$(realp "$3")" $UARGS | head -1; snap_unit $lab; fi
rtk $lab cdx $n $XENV > "$LOUT/$lab.$n.cdx" || exit 1
echo "$n: ordinal $(grep -m1 -o -E 'p[12](cand|dec) phase=p[12] proc=[0-9]+' "$LOUT/$lab.$n.cdx" | sed 's/.*proc=//'), $(grep -c 'p1dec' "$LOUT/$lab.$n.cdx") p1 + $(grep -c 'p2dec' "$LOUT/$lab.$n.cdx") p2 decisions; raw $LOUT/$lab.$n.cdx"
if [ -n "$RAW" ]; then cat "$LOUT/$lab.$n.cdx"; else sh "$TK/sum.sh" "$LOUT/$lab.$n.cdx"; fi
