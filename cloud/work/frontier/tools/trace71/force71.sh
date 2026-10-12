#!/bin/sh
# usage: SCR=.. force71.sh SRC FN SPEC   -- colouring oracle on 7.1: CDX_FN=FN CDX_FORCE=SPEC, stock-equivalent ugen+as1,
# then the forced/declined records and the row diff vs tgt/FN.o (DIFF=1 prints the differing rows).
# SPEC as in the 5.3 toolkit: p1:w6=c14,p2:w3=c7,p1:w9=s (comma-separated, no spaces).
. "$(dirname "$0")/lib/env71.sh"
src=$1; n=$2; spec=$3; b=$(push "$src") || exit 1
rtk force $b $n "'$spec'"
