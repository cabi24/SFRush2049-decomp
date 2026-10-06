#!/bin/sh
# fl.sh FN BODY.c [BASE]: splice BODY into base cluster and score FN
D0=$(pwd)
cd /home/cburnes/projects/rush2049-decomp
W=cloud/work/frontier/w11c
fn=$1; body=$(cd $D0; realpath $2); base=${3:-$W/cu/base.c}
out=$W/tmp_$(basename $body)
python3 $W/tools/fnset.py $base $out $fn $body
python3 -m tools.conveyor.pipeline.blob_unit --tag w11c score $fn --with $out --internal cam_slot_set $EXTRA 2>&1 | grep -E "EQUAL|FAIL|STAGE|rror" | head -3
python3 $W/tools/udiff.py $fn | tail -1
