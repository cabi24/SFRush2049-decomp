#!/bin/sh
# usage: batch.sh DIR  -- each DIR/*.c is a csm body; prefix prepended; scored with bscore (aligned-missing, strict)
D=$(dirname $0); d="$1"; shift
rm -rf $D/_b; mkdir -p $D/_b
for f in $d/*.c; do cat $D/csm_prefix.c $f > $D/_b/$(basename $f); done
ssh watchman2 "rm -rf ~/rush2049/scratch/frontier/w9h/cand/_b"
scp -qr $D/_b watchman2:rush2049/scratch/frontier/w9h/cand/_b && ssh watchman2 "cd ~/rush2049/scratch/frontier/w9h && IDO_DIR=\$HOME/rush2049/cache/toolkits/796ae99a5cb7922e335e3afd87008753c573c82b45d04e8cfbce4f513cdbfbf5/ido python3 cand/bfull.py cand/_b camera_scene_manager --keep camera_scene_manager,func_800C3578 $*"
