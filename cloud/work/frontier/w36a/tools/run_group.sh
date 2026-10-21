#!/bin/bash
# usage: run_group.sh DIR   -- copy group dir to builder and score
d=$1; n=$(basename $d)
S=rush2049/scratch/frontier/w35b
ssh watchman2 "mkdir -p $S/cand/$n" && scp -q $d/group.c $d/group.json watchman2:$S/cand/$n/ && \
ssh watchman2 "cd $S && IDO_DIR=\$HOME/rush2049/cache/toolkits/796ae99a5cb7922e335e3afd87008753c573c82b45d04e8cfbce4f513cdbfbf5/ido python3 tools/cloud/score.py group cand/$n --targets asm/us/ovl_a" 2>&1 | cut -c1-200 | grep -v "^    +0x\|not verified"
