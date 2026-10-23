#!/bin/sh
# usage: de.sh BODY.c [extra blob_unit args] -> assemble pgs/de/head.c + BODY + tail.c, unit-score display_enable (+pgs) and print summary
D=/home/cburnes/projects/rush2049-decomp/cloud/work/frontier/w40b/pgs/de
B=$1; shift
n=$(basename $B .c)
cat $D/head.c $B $D/tail.c > $D/full_$n.c
cd /home/cburnes/projects/rush2049-decomp
python3 -m tools.conveyor.pipeline.blob_unit --tag w40b --jobs 2 score display_enable playgame_state_change --with $D/full_$n.c --with cloud/work/frontier/w40b/pgs/halfont.c --internal display_enable --internal _GetSemaphore_HT --internal _ReleaseSemaphore_HT --keep object_create --internal func_800C9150 --internal func_800CA2F8 "$@" 2>&1 | grep -E "(EQUAL|FAIL) |locked|rror" | cut -c1-120
for f in display_enable playgame_state_change; do echo "$f: $(python3 cloud/work/frontier/tools/trace/udiff.py $f --obj build/blob_unit/w40b/unit.o --summary 2>&1 | tail -1)"; done
