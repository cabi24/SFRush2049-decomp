#!/bin/sh
# usage: TAG=.. pretrace.sh LABEL NAME  -- PRE / register-candidate report for NAME on snapshot st_LABEL:
# uopt's level-3 listing (per-block PRE vectors, @ iscolored), the W5D bit/regcand trace and the colouring trace,
# fetched to $LOUT/pre_LABEL/{list,w5d,cdx}; prints lib/prereport.py (expression text per bit = web number).
# Compare two runs by expression text: python3 lib/precmp.py DIR_A DIR_B NAME.
. "$(dirname "$0")/lib/env.sh"
lab=$1; n=$2; O=$LOUT/pre_$lab; mkdir -p $O
rtk $lab pre $n >/dev/null || exit 1
for f in list w5d cdx; do fetch $lab $f $O/$f; done
python3 "$TK/lib/prereport.py" $O $n ${PREARGS:-} | tee $O/report.txt
