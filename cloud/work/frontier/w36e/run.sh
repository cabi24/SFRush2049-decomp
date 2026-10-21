#!/bin/bash
# usage: run.sh group <dir-under-lane> | fn <file.c> NAME [flags]
# syncs the lane to the builder scratch and scores there.
cd /home/cburnes/projects/rush2049-decomp || exit 1
L=cloud/work/frontier/w36e
B=watchman2:rush2049/scratch/frontier/w36e
IDO='IDO_DIR=$HOME/rush2049/cache/toolkits/796ae99a5cb7922e335e3afd87008753c573c82b45d04e8cfbce4f513cdbfbf5/ido'
rsync -aq --exclude 'v/' --exclude '*.pyc' $L/ $B/lane/ || exit 1
case "$1" in
  group) ssh watchman2 "cd ~/rush2049/scratch/frontier/w36e && $IDO python3 tools/cloud/score.py group lane/$2 --targets asm/us/ovl_a" ;;
  fn) FL="${4:--g0 -O3 -mips2 -G 0 -non_shared}"; ssh watchman2 "cd ~/rush2049/scratch/frontier/w36e && $IDO python3 tools/cloud/score.py fn lane/$2 $3 --targets asm/us/ovl_a --flags '$FL'" ;;
esac
