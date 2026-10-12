#!/bin/sh
# usage: SCR=.. [SCHED=1] ugt71.sh SRC FN   -- 7.1 ugen temp ring: FN's DKWB-FREELIST records (reg = MIPS number,
# emitted index, source line), SCHED=1 adds DKWB-EMIT-V1 (ugen emission order), then FN's ugen listing (cc -S).
. "$(dirname "$0")/lib/env71.sh"
src=$1; n=$2; b=$(push "$src") || exit 1
rtk ugen $b $n | tee "$LOUT/ugen.$n.txt"
