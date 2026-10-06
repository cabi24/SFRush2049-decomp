#!/bin/sh
# flt.sh FN BODY.c PROC [webs-regex]: splice BODY, score, trace PROC colours
D0=$(pwd)
W=/home/cburnes/projects/rush2049-decomp/cloud/work/frontier/w11c
fn=$1; body=$(cd $D0; realpath $2); proc=$3
lab=$(basename $body .c)
python3 $W/tools/fnset.py $W/cu/base.c $W/tmp_$lab.c $fn $body
cd $W && EXTRA="--internal cam_slot_set" sh tools/ctrace.sh $fn tmp_$lab.c $lab $proc
cd /home/cburnes/projects/rush2049-decomp && python3 $W/tools/udiff.py $fn | tail -1
