#!/bin/sh
# usage: TAG=w35c force.sh LABEL NAME SPEC [udiff args]  -- like the toolkit force.sh but diffs against ovl_a
. /home/cburnes/projects/rush2049-decomp/cloud/work/frontier/tools/trace/lib/env.sh
TK=/home/cburnes/projects/rush2049-decomp/cloud/work/frontier/tools/trace
lab=$1; n=$2; spec=$3; shift 3
rtk $lab force $n "'$spec'" || exit 1
fetch $lab f.o "$LOUT/f_$lab.o" && python3 /home/cburnes/projects/rush2049-decomp/cloud/work/frontier/w35c/tools/udiff_ovl.py $n --obj "$LOUT/f_$lab.o" "$@"
