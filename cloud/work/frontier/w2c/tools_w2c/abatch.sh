#!/bin/sh
# usage: abatch.sh NAME FILE.c... -- whole-program unit score + aligned (udiff) row count and frame for each candidate, two lanes
D=$(pwd); cd /home/cburnes/projects/rush2049-decomp
name="$1"; shift
run() { tag=$1; shift; for f in "$@"; do p=$(cd $D; realpath $f); r=$(python3 -m tools.conveyor.pipeline.blob_unit --tag $tag score $name --with "$p" 2>&1 | grep -E "EQUAL|FAIL|rror" | head -1 | cut -c1-60); u=$(python3 cloud/work/frontier/w2c/udiff.py $name --obj build/blob_unit/$tag/unit.o --all 2>&1); fr=$(echo "$u" | grep -m1 "addiu sp,sp" | awk '{print $NF}'); rows=$(echo "$u" | grep -o "differing rows [0-9]*"); echo "$f: $rows frame=$fr |$r"; done; }
n=$#; h=$(( (n+1)/2 )); i=0; A=""; B=""
for f in "$@"; do i=$((i+1)); if [ $i -le $h ]; then A="$A $f"; else B="$B $f"; fi; done
run w2c $A & run w2cb $B & wait
