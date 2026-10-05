#!/bin/sh
# u.sh NAME cand.c [blob_unit args] -- unit score summary line + udiff count
D=$(pwd); name=$1; c=$(cd $D; realpath $2); shift; shift
cd /home/cburnes/projects/rush2049-decomp
python3 -m tools.conveyor.pipeline.blob_unit --tag w5a score $name --with "$c" "$@" 2>&1 | grep -E "EQUAL|FAIL|rror" | head -3
