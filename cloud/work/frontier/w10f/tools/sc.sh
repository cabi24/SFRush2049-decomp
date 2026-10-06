#!/bin/bash
# usage: sc.sh cand.c NAME [flags]   -- score a single on the builder lane w10f
f=$1; n=$2; fl=${3:-"-g0 -O3 -mips2 -G 0 -non_shared"}
b=$(basename $f .c)
ssh watchman2 "mkdir -p ~/rush2049/scratch/frontier/w10f/cand/$n" && scp -q $f watchman2:rush2049/scratch/frontier/w10f/cand/$n/$b.c && \
ssh watchman2 "cd ~/rush2049/scratch/frontier/w10f && IDO_DIR=\$HOME/rush2049/cache/toolkits/796ae99a5cb7922e335e3afd87008753c573c82b45d04e8cfbce4f513cdbfbf5/ido python3 tools/cloud/score.py fn cand/$n/$b.c $n --flags '$fl' ${DIFF:+--diff}"
