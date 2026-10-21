#!/bin/bash
# usage: fn.sh FILE.c NAME [gscore args]  -> standalone compile + score on builder (image B targets)
F=$1; N=$2; shift 2
B=$(basename $F)
rsync -aq /home/cburnes/projects/rush2049-decomp/cloud/work/frontier/w36h/tools/gscore.py watchman2:rush2049/scratch/frontier/w36h/tools/gscore.py
scp -q $F watchman2:rush2049/scratch/frontier/w36h/cand/_fn_$B
ssh watchman2 "cd ~/rush2049/scratch/frontier/w36h && IDO_DIR=\$HOME/rush2049/cache/toolkits/796ae99a5cb7922e335e3afd87008753c573c82b45d04e8cfbce4f513cdbfbf5/ido python3 tools/gscore.py fn cand/_fn_$B $N $*" 2>&1 | grep -v '^compiled'
