#!/bin/sh
# usage: fbx.sh NAME REGEX FILE.c... -- like fb.sh, also print got-column lines matching REGEX (full --all)
name=$1; rx=$2; shift; shift
d=fbx_$$
ssh watchman2 "mkdir -p ~/rush2049/scratch/frontier/w8d/cand/$d"
tar -cf - "$@" | ssh watchman2 "tar -C ~/rush2049/scratch/frontier/w8d/cand/$d -xf -"
ssh watchman2 "cd ~/rush2049/scratch/frontier/w8d && export IDO_DIR=\$HOME/rush2049/cache/toolkits/796ae99a5cb7922e335e3afd87008753c573c82b45d04e8cfbce4f513cdbfbf5/ido; find cand/$d -name \"*.c\" | sed 's#cand/$d/##' | xargs -P 2 -I{} sh -c 'python3 cand/full.py cand/$d/{} $name --all > cand/$d/{}.out 2>&1; r=\$(grep -E \"differing rows|rror\" cand/$d/{}.out | head -1 | sed \"s/.*differing rows/rows/\"); x=\$(grep -E \"$rx\" cand/$d/{}.out | cut -c40- | tr -s \" \" | tr \"\\n\" \";\"); echo \"{}: \$r | \$x\"' | sort; rm -rf cand/$d"
