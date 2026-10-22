#!/bin/bash
# usage: sc.sh NAME file.c [flags]   -> score.py fn on the builder (scratch w39c)
N=$1; F=$2; FL=${3:-"-g0 -O3 -mips2 -G 0 -non_shared"}
B=$(basename $F)
scp -q $F watchman2:rush2049/scratch/frontier/w39c/cand/$B
ssh watchman2 "cd ~/rush2049/scratch/frontier/w39c && IDO_DIR=\$HOME/rush2049/cache/toolkits/796ae99a5cb7922e335e3afd87008753c573c82b45d04e8cfbce4f513cdbfbf5/ido python3 tools/cloud/score.py fn cand/$B $N --flags \"$FL\"" 2>&1 | tail -${TAILN:-3}
