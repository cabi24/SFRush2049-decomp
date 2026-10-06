#!/bin/sh
# fl.sh FN BODY.c [BASE]: splice BODY into base cluster file and unit-score FN (rows from udiff)
D0=$(pwd)
cd /home/cburnes/projects/rush2049-decomp
W=cloud/work/frontier/w12f
fn=$1; body=$(cd $D0; realpath $2); base=${3:-$W/base2.c}
out=$W/tmp/$(basename $body)
mkdir -p $W/tmp
python3 $W/tools/fnset.py $base $out $fn $body
python3 -m tools.conveyor.pipeline.blob_unit --tag w12f score $fn --with $out $EXTRA 2>&1 | grep -E "EQUAL|FAIL|STAGE|rror|locked bodies" | head -4
python3 cloud/work/frontier/tools/trace/udiff.py $fn --obj build/blob_unit/w12f/unit.o $UD | tail -${N:-1}
