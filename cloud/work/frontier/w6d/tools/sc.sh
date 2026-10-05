#!/bin/sh
# usage: sc.sh cand.c NAME [extra score.py args]
src="$1"; name="$2"; shift; shift
scp -q "$src" watchman2:rush2049/scratch/frontier/w6d/cand/$name.c && ssh watchman2 "cd ~/rush2049/scratch/frontier/w6d && IDO_DIR=\$HOME/rush2049/cache/toolkits/796ae99a5cb7922e335e3afd87008753c573c82b45d04e8cfbce4f513cdbfbf5/ido python3 tools/cloud/score.py fn cand/$name.c $name $*"
