#!/bin/sh
# ON BUILDER (cand/): asmbatch_remote.sh DIR NAME -> for each DIR/*.s print "rows name"
for f in $1/*.s; do printf "%s " $(basename $f); sh asm_remote.sh $f $2 | tail -1; done
