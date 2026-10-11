#!/bin/sh
# lst.sh NAME FILE.c LABEL : unit-score FILE, snapshot, fetch ugen listing to build/trace/w20d/fn_LABEL.NAME.s
cd /home/cburnes/projects/rush2049-decomp
export TAG=w20d
T=cloud/work/frontier/tools/trace
n=$1; f=$2; l=$3
$T/us.sh $n $f
$T/snap.sh $l >/dev/null 2>&1
$T/as1t.sh $l $n >/dev/null 2>&1
echo "listing build/trace/w20d/fn_$l.$n.s"
