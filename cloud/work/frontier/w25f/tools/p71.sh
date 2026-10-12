#!/bin/sh
# Pi side: [DIFF=1] p71.sh SRC FN [ENV=V ...]  -> builder pre71r.sh; fetches p71.txt to $LOUT/FN.p71.txt
S=rush2049/scratch/frontier/w25f
LOUT=${LOUT:-/home/cburnes/projects/rush2049-decomp/build/trace71/w25f}; mkdir -p $LOUT
src=$1; fn=$2; shift 2
b=t71/in/$(basename $src .c)_$(echo "$(realpath $src)" | md5sum | cut -c1-6).c
scp -q "$(dirname $0)/pre71r.sh" watchman2:$S/t71/ && scp -q "$src" watchman2:$S/$b || exit 1
ssh watchman2 "DIFF='$DIFF' sh ~/$S/t71/pre71r.sh ~/$S ~/$S/$b $fn $*"
scp -q watchman2:$S/t71/run/$fn/p71.txt $LOUT/$fn.p71.txt
