#!/bin/sh
# usage: ubatch.sh NAME FILE.c... -- whole-program unit score of NAME for each candidate (two lanes in parallel)
D=$(pwd); cd /home/cburnes/projects/rush2049-decomp
name="$1"; shift
run() { tag=$1; shift; for f in "$@"; do r=$(python3 -m tools.conveyor.pipeline.blob_unit --tag $tag score $name --with "$(cd $D; realpath $f)" 2>&1 | grep -E "EQUAL|FAIL|rror" | head -1); echo "$f: $r"; done; }
n=$#; h=$(( (n+1)/2 )); i=0; A=""; B=""
for f in "$@"; do i=$((i+1)); if [ $i -le $h ]; then A="$A $f"; else B="$B $f"; fi; done
run w2c $A & run w2cb $B & wait
