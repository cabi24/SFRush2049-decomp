#!/bin/sh
# usage: gfull.sh group.c NAME keep1,keep2 [--all] -- aligned diff of NAME compiled as -O3 group
src="$1"; name="$2"; keep="$3"; shift; shift; shift
scp -q "$src" watchman2:rush2049/scratch/frontier/w2e/cand/_g_$name.c && ssh watchman2 "cd ~/rush2049/scratch/frontier/w2e && IDO_DIR=\$HOME/rush2049/cache/toolkits/796ae99a5cb7922e335e3afd87008753c573c82b45d04e8cfbce4f513cdbfbf5/ido python3 cand/full.py cand/_g_$name.c $name --flags '-g0 -O3 -mips2 -G 0 -non_shared' --keep $keep $*"
