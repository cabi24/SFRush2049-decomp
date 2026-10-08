#!/bin/sh
# usage: sc.sh FILE.c NAME  -- standalone score.py fn on the builder (prints full diff)
f="$1"; n="$2"; b=$(basename "$f")
scp -q "$f" "watchman2:rush2049/scratch/frontier/w14u/cand_$b" && ssh watchman2 "cd ~/rush2049/scratch/frontier/w14u && IDO_DIR=\$HOME/rush2049/cache/toolkits/796ae99a5cb7922e335e3afd87008753c573c82b45d04e8cfbce4f513cdbfbf5/ido python3 tools/cloud/score.py fn cand_$b $n --flags '-g0 -O3 -mips2 -G 0 -non_shared' 2>&1 | tail -${3:-60}"
