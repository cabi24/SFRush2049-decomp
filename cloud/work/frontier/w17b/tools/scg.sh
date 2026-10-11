#!/bin/sh
# scg.sh FILE.c [extra blob_unit args]: unit-score the 10-function group (4 bodies + 6 stub helpers), neighbours
f=$(realpath $1); shift
cd /home/cburnes/projects/rush2049-decomp
S="func_800E397C func_800E3984 func_800E42E8 func_800E42F0 func_800E42F8 func_800E4B50"; I=""
for x in func_800E451C func_800E4300 func_800E398C $S; do I="$I --internal $x"; done
python3 -m tools.conveyor.pipeline.blob_unit --tag w17b score func_800E4B58 func_800E451C func_800E4300 func_800E398C $S --with $f $I --neighbours "$@" 2>&1 | grep -vE "^\s+\+0x|defined by"
