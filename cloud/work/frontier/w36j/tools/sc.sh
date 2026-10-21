#!/bin/bash
# usage: sc.sh IMG(a|b) FILE.c FUNC [FLAGS...]   (scores on builder scratch w36j)
IMG=$1; F=$2; FN=$3; shift 3
FLAGS=${*:-"-g0 -O3 -mips2 -G 0 -non_shared"}
B=$(basename $F)
scp -q $F watchman2:rush2049/scratch/frontier/w36j/cand/$B
ssh watchman2 "cd ~/rush2049/scratch/frontier/w36j && IDO_DIR=\$HOME/rush2049/cache/toolkits/796ae99a5cb7922e335e3afd87008753c573c82b45d04e8cfbce4f513cdbfbf5/ido python3 tools/cloud/score.py fn cand/$B $FN --targets asm/us/ovl_$IMG --flags '$FLAGS'"
