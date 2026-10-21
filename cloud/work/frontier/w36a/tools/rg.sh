#!/bin/bash
# usage: rg.sh DIR [extra score args]  -- copy group dir to builder (w36a scratch) and score it; full output
d=$1; shift; n=$(basename $d)
S=rush2049/scratch/frontier/w36a
ssh watchman2 "mkdir -p $S/cand/$n" && scp -q $d/group.c $d/group.json watchman2:$S/cand/$n/ && \
ssh watchman2 "cd $S && IDO_DIR=\$HOME/rush2049/cache/toolkits/796ae99a5cb7922e335e3afd87008753c573c82b45d04e8cfbce4f513cdbfbf5/ido python3 tools/cloud/score.py group cand/$n --targets asm/us/ovl_a $*" 2>&1
