#!/bin/bash
# usage: run.sh body.c  -> assembles group, scores on builder
set -e
W=/home/cburnes/projects/rush2049-decomp/cloud/work/frontier/w9a
G=$W/groups/steering_sensitivity
cat $W/pre.c $1 $W/post.c > $G/group.c
rsync -a $G/ watchman2:rush2049/scratch/frontier/w9a/cand/steer/
ssh watchman2 'cd ~/rush2049/scratch/frontier/w9a && IDO_DIR=$HOME/rush2049/cache/toolkits/796ae99a5cb7922e335e3afd87008753c573c82b45d04e8cfbce4f513cdbfbf5/ido taskset -c 0,1 python3 tools/cloud/score.py group cand/steer 2>&1' | tail -${2:-8}
