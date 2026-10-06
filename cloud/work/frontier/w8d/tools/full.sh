#!/bin/sh
# usage: full.sh cand.c NAME [full.py args] -- aligned full diff on the builder (w8d scratch)
src="$1"; name="$2"; shift; shift
scp -q "$src" watchman2:rush2049/scratch/frontier/w8d/cand/_f_$name.c && ssh watchman2 "cd ~/rush2049/scratch/frontier/w8d && IDO_DIR=\$HOME/rush2049/cache/toolkits/796ae99a5cb7922e335e3afd87008753c573c82b45d04e8cfbce4f513cdbfbf5/ido python3 cand/full.py cand/_f_$name.c $name $*"
