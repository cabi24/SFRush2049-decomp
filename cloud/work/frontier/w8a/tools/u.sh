#!/bin/sh
# u.sh NAME FILES... -- unit-score each candidate (tag w8a), print the result line
R=/home/cburnes/projects/rush2049-decomp; D=$(pwd); N=$1; shift
for f in "$@"; do p=$(cd $D; realpath $f); r=$(cd $R && python3 -m tools.conveyor.pipeline.blob_unit --tag w8a score $N $EXTRA --with $p 2>&1 | grep -E "EQUAL|FAIL|rror" | head -1); echo "$f:$r"; done
