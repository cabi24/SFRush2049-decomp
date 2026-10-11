#!/bin/sh
# w20b (builder): diagnose verdict line for each FN's D10 source
for f in "$@"; do p=$(sh lane/tools/d10.sh $f); [ -f "$p" ] || continue
 printf "%s " $f; sh lane/tools/diag.sh $p $f 2>&1 | grep -E "^verdict=" | cut -c1-110; done
