#!/bin/sh
# usage: sc.sh cand.c NAME   -- strict-score a candidate on the builder (w1a copy only)
scp -q "$1" watchman2:rush2049/scratch/frontier/w1a/cand/$2.c
ssh -o BatchMode=yes watchman2 "cd ~/rush2049/scratch/frontier/w1a && IDO_DIR=\$HOME/rush2049/cache/toolkits/796ae99a5cb7922e335e3afd87008753c573c82b45d04e8cfbce4f513cdbfbf5/ido python3 tools/cloud/score.py fn cand/$2.c $2 --flags \"${3:--g0 -O3 -mips2 -G 0 -non_shared}\""
