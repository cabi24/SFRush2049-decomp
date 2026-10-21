#!/bin/bash
# usage: gdp.sh GROUPDIR(Pi) FN -> sp offsets of FN
N=$(basename $1)
ssh watchman2 "rm -rf ~/rush2049/scratch/frontier/w36f/cand/g_$N && mkdir -p ~/rush2049/scratch/frontier/w36f/cand/g_$N"
rsync -a $1/ watchman2:rush2049/scratch/frontier/w36f/cand/g_$N/
scp -q $(dirname $0)/gdump.py watchman2:rush2049/scratch/frontier/w36f/gdump.py
ssh watchman2 "cd ~/rush2049/scratch/frontier/w36f && IDO_DIR=\$HOME/rush2049/cache/toolkits/796ae99a5cb7922e335e3afd87008753c573c82b45d04e8cfbce4f513cdbfbf5/ido python3 gdump.py cand/g_$N $2 a"
