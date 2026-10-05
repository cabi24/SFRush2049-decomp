#!/bin/bash
# usage: sg.sh <groupname> [extra score args]  -- pushes cloud/work/frontier/w1g/groups/<g> and scores it on the builder
g=$1; shift
cd /home/cburnes/projects/rush2049-decomp/cloud/work/frontier/w1g/groups
rsync -a --delete $g/ watchman2:rush2049/scratch/frontier/w1g/cand/$g/
ssh watchman2 "cd ~/rush2049/scratch/frontier/w1g && IDO_DIR=\$HOME/rush2049/cache/toolkits/796ae99a5cb7922e335e3afd87008753c573c82b45d04e8cfbce4f513cdbfbf5/ido taskset -c 12,13 python3 tools/cloud/score.py group cand/$g $*"
