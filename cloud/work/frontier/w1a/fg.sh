#!/bin/sh
# usage: fg.sh SRC.c FN [KEEP,KEEP] [extra full.py args] -- aligned diff of FN on the builder (w1a copy); -O3, group if KEEP given
src="$1"; name="$2"; keep="$3"; shift; shift; shift
scp -q "$src" watchman2:rush2049/scratch/frontier/w1a/cand/_f_$name.c && ssh -o BatchMode=yes watchman2 "cd ~/rush2049/scratch/frontier/w1a && IDO_DIR=\$HOME/rush2049/cache/toolkits/796ae99a5cb7922e335e3afd87008753c573c82b45d04e8cfbce4f513cdbfbf5/ido python3 cand/full.py cand/_f_$name.c $name --flags '-g0 -O3 -mips2 -G 0 -non_shared' ${keep:+--keep $keep} $*"
