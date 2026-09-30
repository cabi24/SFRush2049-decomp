#!/bin/bash
# ok.sh name [flags]: copy to matches if strict bare MATCH
cd /home/user/SFRush2049-decomp/cloud/work/newtargets_lo
n=$1; F="${2:--g0 -O2 -mips2 -G 0 -non_shared}"
out=$(LINES_N=3 ./mk.sh $n "$F")
echo "$out"
if [ "$(echo "$out" | tail -1 | tr -d ' ')" = "MATCH" ]; then
 ( echo "/* flags: $F */"; cat $n.c ) > ../../matches/$n.c; echo COPIED; fi
