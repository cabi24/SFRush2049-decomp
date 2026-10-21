#!/bin/bash
# fs.sh FILE NAME [flags]: standalone score via gscore fn on builder, summary line + optional diff args
F=$1; N=$2; shift 2
S=watchman2:rush2049/scratch/frontier/w36l
rsync -aq /home/cburnes/projects/rush2049-decomp/cloud/work/frontier/w36l/tools/gscore.py $S/tools/gscore.py
scp -q $F $S/cand/$(basename $F)
ssh watchman2 "cd ~/rush2049/scratch/frontier/w36l && IDO_DIR=\$HOME/rush2049/cache/toolkits/796ae99a5cb7922e335e3afd87008753c573c82b45d04e8cfbce4f513cdbfbf5/ido python3 tools/gscore.py fn cand/$(basename $F) $N --targets asm/us/blob $*"
