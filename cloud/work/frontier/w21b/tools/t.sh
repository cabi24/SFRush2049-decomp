#!/bin/sh
# w21b (Pi): push SRC (path under the lane dir) to the builder lane copy, print LOCK line (+ diff rows with -d)
# usage: sh tools/t.sh SRC FN [-d]   (run from the lane dir)
src=$1; fn=$2
ssh watchman2 "mkdir -p rush2049/scratch/frontier/w21b/lane/$(dirname $src)" && scp -q $src watchman2:rush2049/scratch/frontier/w21b/lane/$src
ssh watchman2 "cd ~/rush2049/scratch/frontier/w21b && sh lane/tools/sc.sh lane/$src $fn 2>&1 | grep -E '^(LOCK|TU):' ; if [ '$3' = -d ]; then sh lane/tools/dif.sh lane/$src $fn | grep -n -E '\s[|]\s|\s<$|\s>\s'; fi"
