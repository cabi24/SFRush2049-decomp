#!/bin/sh
# w20a (Pi): push SRC to builder lane copy, print LOCK line (+ diff rows with -d)
# usage: sh tools/t.sh SRC FN [-d]
L=/home/cburnes/projects/rush2049-decomp/cloud/work/frontier/w20a
src=$1; fn=$2; rel=${src#$L/}; rel=${rel#cloud/work/frontier/w20a/}
rsync -aR $(realpath --relative-to=$L $src 2>/dev/null || echo $rel) watchman2:rush2049/scratch/frontier/w20a/lane/ 2>/dev/null || (cd $L && rsync -aR $rel watchman2:rush2049/scratch/frontier/w20a/lane/)
ssh watchman2 "cd ~/rush2049/scratch/frontier/w20a && sh lane/tools/sc.sh lane/$rel $fn 2>&1 | grep -E '^(LOCK|TU):' ; if [ '$3' = -d ]; then sh lane/tools/dif.sh lane/$rel $fn | grep -n -E '\s[|]\s|\s<$|\s>\s'; fi"
