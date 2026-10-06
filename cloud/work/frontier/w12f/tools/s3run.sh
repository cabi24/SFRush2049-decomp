#!/bin/sh
# s3run.sh FILE.c... : score a function body FILE (FN, default camera_update) inside $BASE (default base2.c)
W=/home/cburnes/projects/rush2049-decomp/cloud/work/frontier/w12f; mkdir -p $W/tmp
for f in "$@"; do b=$(basename $f .c); python3 $W/tools/fnset.py $W/${BASE:-base2.c} $W/tmp/s3_$b.c ${FN:-camera_update} $f
(cd /home/cburnes/projects/rush2049-decomp; printf "%s: " $b; python3 -m tools.conveyor.pipeline.blob_unit --tag w12f score ${FN:-camera_update} $EXTRA --with $W/tmp/s3_$b.c 2>&1 | grep -E "FAIL|EQUAL|rror" | head -2 | tr '\n' ' '; python3 cloud/work/frontier/tools/trace/udiff.py ${FN:-camera_update} --obj build/blob_unit/w12f/unit.o | tail -1); done
