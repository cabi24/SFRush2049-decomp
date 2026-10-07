#!/bin/sh
# s3n.sh FILE.c... : like s3run.sh but also prints nosp rows and the tt/na/tb registers
W=/home/cburnes/projects/rush2049-decomp/cloud/work/frontier/w13c; mkdir -p $W/tmp
for f in "$@"; do b=$(basename $f .c); python3 $W/tools/fnset.py $W/${BASE:-base2.c} $W/tmp/s3_$b.c ${FN:-camera_update} $f
(cd /home/cburnes/projects/rush2049-decomp; printf "%s: " $b; python3 -m tools.conveyor.pipeline.blob_unit --tag w13c score ${FN:-camera_update} $EXTRA --with $W/tmp/s3_$b.c 2>&1 | grep -E "FAIL|EQUAL|rror" | head -2 | sed 's/camera_update: //' | tr '\n' ' '; python3 cloud/work/frontier/tools/trace/udiff.py ${FN:-camera_update} --obj build/blob_unit/w13c/unit.o | tail -1 | sed 's/.*differing/rows/;s/unverified.*//'| tr '\n' ' '; python3 cloud/work/frontier/tools/trace/udiff.py ${FN:-camera_update} --obj build/blob_unit/w13c/unit.o --nosp | tail -1 | sed 's/.*differing rows/nosp/;s/;.*//'); done
