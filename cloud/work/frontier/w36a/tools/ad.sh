#!/bin/bash
# usage: ad.sh GROUPDIR FN [adiff args]  -- copy group to builder and run aligned diff for FN
d=$1; f=$2; shift 2; n=$(basename $d)
S=rush2049/scratch/frontier/w36a
ssh watchman2 "mkdir -p $S/cand/$n" && scp -q $d/group.c $d/group.json watchman2:$S/cand/$n/ && \
ssh watchman2 "cd $S && IDO_DIR=\$HOME/rush2049/cache/toolkits/796ae99a5cb7922e335e3afd87008753c573c82b45d04e8cfbce4f513cdbfbf5/ido python3 adiff.py x $f --group cand/$n $*"
