#!/bin/sh
# usage: [UARGS=..] ubatch.sh DIR NAME -- score each DIR/*.c in the whole-program unit (sequential, ~5 s each)
cd /home/cburnes/projects/rush2049-decomp
for f in $1/*.c; do r=$(python3 -m tools.conveyor.pipeline.blob_unit --tag w2f score $2 --with $f $UARGS 2>&1 | grep -E "EQUAL|FAIL|rror" | head -1); echo "$(basename $f): $r"; done
