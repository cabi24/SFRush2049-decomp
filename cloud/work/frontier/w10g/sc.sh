#!/bin/sh
# usage: sc.sh NAME file.c [flags]
N=$1; F=$2; FL=${3:-"-g0 -O3 -mips2 -G 0 -non_shared"}
scp -q "$F" watchman2:rush2049/scratch/frontier/w10g/cand/$N.c
ssh watchman2 "cd ~/rush2049/scratch/frontier/w10g && IDO_DIR=\$HOME/rush2049/cache/toolkits/796ae99a5cb7922e335e3afd87008753c573c82b45d04e8cfbce4f513cdbfbf5/ido python3 tools/cloud/score.py fn cand/$N.c $N --flags \"$FL\""
