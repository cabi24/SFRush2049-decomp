#!/bin/bash
# rd.sh <diff.py args>: sync and run diff.py remotely
L=/home/cburnes/projects/rush2049-decomp/cloud/work/s20261004/A
R='~/rush2049/scratch/s20261004_A'
tar cf - -C $L . | ssh watchman2 "mkdir -p $R/A && cd $R/A && tar xf -"
ssh watchman2 "cd $R/A && IDO_DIR=\$HOME/rush2049/repo/tools/ido-static-recomp/build/out python3 diff.py $* 2>&1"
