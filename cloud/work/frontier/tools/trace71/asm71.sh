#!/bin/sh
# usage: SCR=.. [DIFF=1] asm71.sh EDITED.s FN  -- assemble an edited listing (stock 7.1 as0/as1 with -r4300_mul), row diff
# vs tgt/FN.o: answers "is the residual as1's, or does it come from uopt/ugen?"
. "$(dirname "$0")/lib/env71.sh"
s=$1; n=$2; b=$(push "$s") || exit 1
rtk asm $b $n
