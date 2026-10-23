#!/bin/bash
# usage: gs.sh GROUPDIR [extra score.py args]  -> score.py group on the builder (scratch w40a/cand/g/<basename>)
D=$1; shift
B=$(basename $D)
ssh watchman2 "rm -rf ~/rush2049/scratch/frontier/w40a/cand/g/$B; mkdir -p ~/rush2049/scratch/frontier/w40a/cand/g"
scp -q -r $D watchman2:rush2049/scratch/frontier/w40a/cand/g/$B
ssh watchman2 "cd ~/rush2049/scratch/frontier/w40a && IDO_DIR=\$HOME/rush2049/cache/toolkits/796ae99a5cb7922e335e3afd87008753c573c82b45d04e8cfbce4f513cdbfbf5/ido python3 tools/cloud/score.py group cand/g/$B $*" 2>&1 | tail -${TAILN:-12}
