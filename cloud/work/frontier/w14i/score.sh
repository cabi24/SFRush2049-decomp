#!/bin/bash
# usage: score.sh LOCAL.c FN -> copies to builder cand/FN.c and scores -O3 standalone
L=$1; FN=$2; S=rush2049/scratch/frontier/w14i
scp -q "$L" watchman2:$S/cand/$FN.c || exit 1
timeout 110 ssh watchman2 "cd ~/$S && IDO_DIR=\$HOME/rush2049/cache/toolkits/796ae99a5cb7922e335e3afd87008753c573c82b45d04e8cfbce4f513cdbfbf5/ido timeout 100 python3 tools/cloud/score.py fn cand/$FN.c $FN --flags '-g0 -O3 -mips2 -G 0 -non_shared' 2>&1 | tail -${3:-3}"
