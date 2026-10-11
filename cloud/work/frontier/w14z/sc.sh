#!/bin/sh
# usage: sc.sh LOCAL.c FN [FLAGS...]  -- scores one candidate standalone on watchman2 (scratch w14z)
F=$1; FN=$2; B=$(basename "$F")
scp -q "$F" watchman2:rush2049/scratch/frontier/w14z/cand/"$B" || exit 1
ssh watchman2 "cd ~/rush2049/scratch/frontier/w14z && IDO_DIR=\$HOME/rush2049/cache/toolkits/796ae99a5cb7922e335e3afd87008753c573c82b45d04e8cfbce4f513cdbfbf5/ido timeout 900 python3 tools/cloud/score.py fn cand/$B $FN --flags '-g0 -O3 -mips2 -G 0 -non_shared' 2>&1 | tail -${TAILN:-3}"
