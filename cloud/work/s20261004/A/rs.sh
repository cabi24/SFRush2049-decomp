#!/bin/bash
# rs.sh <score.py args...>: sync cloud/work/s20261004/A to private builder dir and run score.py there
set -e
L=/home/cburnes/projects/rush2049-decomp/cloud/work/s20261004/A
R='~/rush2049/scratch/s20261004_A'
tar cf - -C $L . | ssh watchman2 "mkdir -p $R/A && cd $R/A && tar xf -"
ssh watchman2 "cd $R/repo && IDO_DIR=\$HOME/rush2049/repo/tools/ido-static-recomp/build/out python3 tools/cloud/score.py $* 2>&1" | sed 's#/home/cburnes/rush2049/scratch/s20261004_A/##'
