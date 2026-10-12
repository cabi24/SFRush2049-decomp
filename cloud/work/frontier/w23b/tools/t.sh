#!/bin/sh
# w23b (Pi): push SRC to builder lane copy, print LOCK line (+ diff rows with -d)
# usage: sh tools/t.sh SRC FN [-d]
L=/home/cburnes/projects/rush2049-decomp/cloud/work/frontier/w23b
src=$1; fn=$2; rel=${src#$L/}; rel=${rel#cloud/work/frontier/w23b/}
rsync -aR $(realpath --relative-to=$L $src 2>/dev/null || echo $rel) watchman2:rush2049/scratch/frontier/w23b/lane/ 2>/dev/null || (cd $L && rsync -aR $rel watchman2:rush2049/scratch/frontier/w23b/lane/)
ssh watchman2 "cd ~/rush2049/scratch/frontier/w23b && sh lane/tools/sc.sh lane/$rel $fn 2>&1 | grep -E '^(LOCK|TU):' ; if [ '$3' = -d ]; then sh lane/tools/dif.sh lane/$rel $fn | grep -n -E '\s[|]\s|\s<$|\s>\s'; fi"
