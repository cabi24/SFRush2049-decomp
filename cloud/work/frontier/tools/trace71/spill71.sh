#!/bin/sh
# usage: SCR=.. spill71.sh SRC FN  -- 7.1 spill homes / local area of FN (uopt f_spilltemps, SPLOG=1); see README.
. "$(dirname "$0")/lib/env71.sh"
src=$1; n=$2; b=$(push "$src") || exit 1
rtk spill $b $n
