#!/bin/sh
# w22a (Pi): non-branch diff rows of lane-relative SRC vs target FN
src=$1; fn=$2
rsync -aR -q /home/cburnes/projects/rush2049-decomp/cloud/work/frontier/w22a/./$src watchman2:rush2049/scratch/frontier/w22a/lane/
ssh watchman2 "cd ~/rush2049/scratch/frontier/w22a && sh lane/tools/dif.sh lane/$src $fn" | grep -nP '\s[|<>](\s|$)' | grep -vP '\t(b|beq|bne|beqz|bnez|beqzl|bnezl|blezl|bnel|beql|bgez|bc1fl|bc1tl|bc1f|bc1t)\s' | grep -v "b[a-z0-9]* .*<func.*b[a-z0-9]* .*<func"
