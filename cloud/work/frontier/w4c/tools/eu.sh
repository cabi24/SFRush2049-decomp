#!/bin/sh
# usage: eu.sh FILE.c... -- score the engine unit (3 functions, engine_torque_calc internal) for each candidate, two lanes
D=$(pwd); cd /home/cburnes/projects/rush2049-decomp
run() { tag=$1; shift; for f in "$@"; do r=$(python3 -m tools.conveyor.pipeline.blob_unit --tag $tag score engine_torque_calc transmission_ratio_get engine_sound_update --with "$(cd $D; realpath $f)" --internal engine_torque_calc $EXTRA 2>&1 | grep -E "FAIL|EQUAL" | sed 's/: \([0-9]*\) of \([0-9]*\) words differ.*/ \1/' | awk '{print $2, $3}' | tr '\n' ' '); echo "$f: $r"; done; }
n=$#; h=$(( (n+1)/2 )); i=0; A=""; B=""
for f in "$@"; do i=$((i+1)); if [ $i -le $h ]; then A="$A $f"; else B="$B $f"; fi; done
run w4c $A & run w4cb $B & wait
