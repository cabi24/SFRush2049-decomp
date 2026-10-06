#!/bin/sh
# usage: sc.sh cand.c NAME [FLAGS] -- standalone score.py fn (default -O3 flags)
src="$1"; name="$2"; fl="${3:--g0 -O3 -mips2 -G 0 -non_shared}"
scp -q "$src" watchman2:rush2049/scratch/frontier/w8d/cand/$name.c && ssh watchman2 "cd ~/rush2049/scratch/frontier/w8d && IDO_DIR=\$HOME/rush2049/cache/toolkits/796ae99a5cb7922e335e3afd87008753c573c82b45d04e8cfbce4f513cdbfbf5/ido python3 tools/cloud/score.py fn cand/$name.c $name --flags '$fl'"
