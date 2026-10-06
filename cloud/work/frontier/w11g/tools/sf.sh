#!/bin/sh
# sf.sh NAME FILE.c... -- standalone score.py fn (-O3) for each file, one line each
N=$1; shift
for f in "$@"; do
  scp -q "$f" watchman2:rush2049/scratch/frontier/w11g/cand/sf_$N.c
  r=$(ssh watchman2 "cd ~/rush2049/scratch/frontier/w11g && IDO_DIR=\$HOME/rush2049/cache/toolkits/796ae99a5cb7922e335e3afd87008753c573c82b45d04e8cfbce4f513cdbfbf5/ido python3 tools/cloud/score.py fn cand/sf_$N.c $N --flags '-g0 -O3 -mips2 -G 0 -non_shared'" 2>&1 | grep -E 'words differ|MATCH' | head -1 | cut -c1-120)
  echo "$(basename $f): $r"
done
