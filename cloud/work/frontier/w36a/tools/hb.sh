#!/bin/bash
# usage: hb.sh VARIANT.c [...]  -- score each A1EAC variant (full function body file) in group g/h0; print A1EAC summary + tail rows
for f in "$@"; do
  n=$(basename $f .c)
  python3 tools/rep.py g/h0 g/hb_$n func_803A1EAC=$f ${SUBS:+--sub "$SUBS"} >/dev/null || { echo "$n: rep failed"; continue; }
  out=$(tools/ad.sh g/hb_$n func_803A1EAC --ctx 0 2>&1 | tail -1)
  echo "$n: $out"
done
