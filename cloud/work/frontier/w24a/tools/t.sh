#!/bin/sh
# w24a (Pi, run from repo root): push lane dir to builder lane/, print LOCK/TU lines for SRC FN (+ diff with -d)
src=$1; fn=$2
rsync -a cloud/work/frontier/w24a/ watchman2:rush2049/scratch/frontier/w24a/lane/
ssh watchman2 "cd ~/rush2049/scratch/frontier/w24a && sh lane/tools/sc.sh lane/$src $fn 2>&1 | grep -E '^(LOCK|TU):' ; if [ '$3' = -d ]; then sh lane/tools/dif.sh lane/$src $fn | grep -n -E '\s[|]\s|\s<$|\s>\s'; fi"
