#!/bin/bash
# gsc.sh GROUPDIR [--claims]: score a group dir on watchman2 (w36o scratch)
d=${1%/}; shift
n=$(basename $d)
ssh watchman2 "rm -rf ~/rush2049/scratch/frontier/w36o/cand/groups/$n; mkdir -p ~/rush2049/scratch/frontier/w36o/cand/groups"
scp -q -r $d watchman2:rush2049/scratch/frontier/w36o/cand/groups/ || exit 1
ssh watchman2 "cd ~/rush2049/scratch/frontier/w36o && IDO_DIR=\$HOME/rush2049/cache/toolkits/796ae99a5cb7922e335e3afd87008753c573c82b45d04e8cfbce4f513cdbfbf5/ido python3 tools/cloud/score.py group cand/groups/$n $*"
