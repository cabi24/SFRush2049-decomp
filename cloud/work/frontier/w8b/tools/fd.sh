#!/bin/sh
# usage: fd.sh cand.c NAME [full.py args] -- aligned diff on builder (default -O3)
src="$1"; name="$2"; shift; shift
scp -q "$src" watchman2:rush2049/scratch/frontier/w8b/cand/fd_$name.c && ssh watchman2 "cd ~/rush2049/scratch/frontier/w8b && IDO_DIR=\$HOME/rush2049/cache/toolkits/796ae99a5cb7922e335e3afd87008753c573c82b45d04e8cfbce4f513cdbfbf5/ido python3 tools/full.py cand/fd_$name.c $name --flags '-g0 -O3 -mips2 -G 0 -non_shared' $*"
