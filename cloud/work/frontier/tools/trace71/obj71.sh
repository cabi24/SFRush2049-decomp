#!/bin/sh
# usage: SCR=.. [DIFF=1] obj71.sh SRC FN  -- plain 7.1 compile (traced passes, tracing off) and the row diff vs tgt/FN.o
. "$(dirname "$0")/lib/env71.sh"
src=$1; n=$2; b=$(push "$src") || exit 1
rtk obj $b $n
