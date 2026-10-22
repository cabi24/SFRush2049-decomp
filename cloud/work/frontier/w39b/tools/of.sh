#!/bin/sh
# of.sh ovl_a|ovl_b FILE.c FN : standalone score.py fn on the builder against overlay targets; also fetch object? (no)
IMG="$1"; F="$2"; FN="$3"
R=rush2049/scratch/frontier/w39b
B=$(basename "$F")
scp -q "$F" watchman2:$R/cand/of_$B
ssh watchman2 "cd ~/$R && IDO_DIR=\$HOME/rush2049/cache/toolkits/796ae99a5cb7922e335e3afd87008753c573c82b45d04e8cfbce4f513cdbfbf5/ido python3 tools/cloud/score.py fn cand/of_$B $FN --targets asm/us/$IMG --flags '-g0 -O3 -mips2 -G 0 -non_shared'"
