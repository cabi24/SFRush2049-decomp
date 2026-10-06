#!/bin/sh
# usage: TAG=.. [SCHED=1] ugt.sh LABEL NAME  -- ugen evaluation order for NAME on snapshot st_LABEL: stock uopt, then the
# traced ugen. Prints NAME's DKWB-FREELIST records (temp-ring pops/pushes: event, reg number, emitted index, source
# line), with SCHED=1 also DKWB-EMIT-V1 records (every emitted instruction in ugen order with its line), then
# NAME's pre-as1 listing (ugen -l). Saved to $LOUT/ugen_LABEL.NAME.txt.
. "$(dirname "$0")/lib/env.sh"
rtk $1 ugen $2 | tee "$LOUT/ugen_$1.$2.txt"
