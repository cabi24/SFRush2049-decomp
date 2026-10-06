#!/bin/sh
# usage: TAG=.. as1t.sh LABEL NAME  -- as1 scheduling trace of NAME: its ugen listing alone -> as0 -> traced as1 -R.
# Fetches the trace to $LOUT/as1r_LABEL.NAME.log: per block the DAG ("Node N: inst XXXXXXXX, relocation, lineno L"
# + "before, aftercycles, maxhazard") and each "Picking node" decision with the ready list (time/aftercycles/latency).
# Ties on aftercycles go to the lower source line (w4a). The listing is in st_LABEL/fn1.s (edit it and use asm.sh).
. "$(dirname "$0")/lib/env.sh"
rtk $1 as1 $2 && fetch $1 as1r.log "$LOUT/as1r_$1.$2.log" && fetch $1 fn1.s "$LOUT/fn_$1.$2.s" && echo "trace $LOUT/as1r_$1.$2.log, listing $LOUT/fn_$1.$2.s"
