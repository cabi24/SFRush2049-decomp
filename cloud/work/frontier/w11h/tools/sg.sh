#!/bin/sh
# usage: sg.sh GROUPDIR  -- score a group dir on the builder scratch
G=$1; n=$(basename $G)
rsync -a --delete $G/ watchman2:rush2049/scratch/frontier/w11h/cand/$n/
ssh watchman2 "cd ~/rush2049/scratch/frontier/w11h && IDO_DIR=\$HOME/rush2049/cache/toolkits/796ae99a5cb7922e335e3afd87008753c573c82b45d04e8cfbce4f513cdbfbf5/ido python3 tools/cloud/score.py group cand/$n" 
