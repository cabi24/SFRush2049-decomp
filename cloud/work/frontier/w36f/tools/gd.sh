#!/bin/bash
# usage: gd.sh group|fn IMG SRC FN [flags]   (SRC is a dir for group, file for fn)
M=$1; IMG=$2; SRC=$3; FN=$4; shift 4
if [ "$M" = group ]; then
  N=$(basename $SRC)
  ssh watchman2 "rm -rf ~/rush2049/scratch/frontier/w36f/cand/g_$N && mkdir -p ~/rush2049/scratch/frontier/w36f/cand/g_$N"
  rsync -a $SRC/ watchman2:rush2049/scratch/frontier/w36f/cand/g_$N/
  P=cand/g_$N
else
  scp -q $SRC watchman2:rush2049/scratch/frontier/w36f/cand/$(basename $SRC); P=cand/$(basename $SRC)
fi
scp -q $(dirname $0)/gdiff.py watchman2:rush2049/scratch/frontier/w36f/gdiff.py
ssh watchman2 "cd ~/rush2049/scratch/frontier/w36f && IDO_DIR=\$HOME/rush2049/cache/toolkits/796ae99a5cb7922e335e3afd87008753c573c82b45d04e8cfbce4f513cdbfbf5/ido FULL=$FULL python3 gdiff.py $M $P $FN $IMG $*"
