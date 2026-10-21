#!/bin/bash
# usage: gs.sh GROUPDIR [--claims]  -> score.py group on the builder (w36w3 scratch)
D=$1; shift
B=$(basename $D)
ssh watchman2 "rm -rf ~/rush2049/scratch/frontier/w36w3/cand/g_$B" && scp -q -r $D watchman2:rush2049/scratch/frontier/w36w3/cand/g_$B || exit 1
ssh watchman2 "cd ~/rush2049/scratch/frontier/w36w3 && IDO_DIR=\$HOME/rush2049/cache/toolkits/796ae99a5cb7922e335e3afd87008753c573c82b45d04e8cfbce4f513cdbfbf5/ido timeout 600 python3 tools/cloud/score.py group cand/g_$B $* 2>&1"
