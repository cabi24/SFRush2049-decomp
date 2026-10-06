#!/bin/sh
# usage: fb.sh NAME FILE.c... -- full.py (-O3) aligned-row count for each candidate, 2 parallel lanes
name=$1; shift
d=fb_$$
ssh watchman2 "mkdir -p ~/rush2049/scratch/frontier/w8d/cand/$d"
tar -cf - "$@" | ssh watchman2 "tar -C ~/rush2049/scratch/frontier/w8d/cand/$d -xf -"
ssh watchman2 "cd ~/rush2049/scratch/frontier/w8d && export IDO_DIR=\$HOME/rush2049/cache/toolkits/796ae99a5cb7922e335e3afd87008753c573c82b45d04e8cfbce4f513cdbfbf5/ido; find cand/$d -name \"*.c\" | sed 's#cand/$d/##' | xargs -P 2 -I{} sh -c 'r=\$(python3 cand/full.py cand/$d/{} $name 2>&1 | grep -E \"differing rows|rror\" | head -1); echo \"{}: \$r\"' | sort; rm -rf cand/$d"
