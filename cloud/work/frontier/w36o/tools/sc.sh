#!/bin/bash
# sc.sh FILE.c NAME [extra score.py args...]: score a candidate standalone on watchman2 (w36o scratch)
f=$1; n=$2; shift 2
b=$(basename $f .c)
scp -q $f watchman2:rush2049/scratch/frontier/w36o/cand/$b.c || exit 1
ssh watchman2 "cd ~/rush2049/scratch/frontier/w36o && IDO_DIR=\$HOME/rush2049/cache/toolkits/796ae99a5cb7922e335e3afd87008753c573c82b45d04e8cfbce4f513cdbfbf5/ido python3 tools/cloud/score.py fn cand/$b.c $n --flags '-g0 -O3 -mips2 -G 0 -non_shared' $*"
