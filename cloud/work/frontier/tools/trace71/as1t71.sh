#!/bin/sh
# usage: SCR=.. as1t71.sh SRC FN   -- 7.1 as1 scheduling trace: FN's ugen listing alone -> as0 -> as1 -R.
# Fetches $LOUT/as1r.FN.log (per block "Node N: inst, relocation, lineno" + before/aftercycles/maxhazard, and the
# picks) and the listing $LOUT/fn.FN.s (edit it, then asm71.sh).
. "$(dirname "$0")/lib/env71.sh"
src=$1; n=$2; b=$(push "$src") || exit 1
rtk as1 $b $n && fetch $n/as1r.log "$LOUT/as1r.$n.log" && fetch $n/fn1.s "$LOUT/fn.$n.s" && echo "trace $LOUT/as1r.$n.log, listing $LOUT/fn.$n.s"
