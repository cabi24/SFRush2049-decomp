#!/bin/sh
# usage: gbatch.sh FUNC localdir -- score every group dir under localdir, print FUNC's result
func="$1"; dir="$2"; base=$(basename $dir)
ssh watchman2 "rm -rf ~/rush2049/scratch/frontier/w8c/cand/g_$base"
scp -qr "$dir" watchman2:rush2049/scratch/frontier/w8c/cand/g_$base
ssh watchman2 "cd ~/rush2049/scratch/frontier/w8c && export IDO_DIR=\$HOME/rush2049/cache/toolkits/796ae99a5cb7922e335e3afd87008753c573c82b45d04e8cfbce4f513cdbfbf5/ido; ls -d cand/g_$base/*/ | xargs -P2 -I{} sh -c 'echo \"{}: \$(python3 tools/cloud/score.py group {} 2>&1 | awk \"/^$func:/{f=1;next} f&&/MATCH|differ/{print;exit}\")\"' | sort"
