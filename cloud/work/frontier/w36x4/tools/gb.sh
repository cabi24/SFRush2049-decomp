#!/bin/bash
# usage: gb.sh GROUPDIR VARDIR FILE NAMES [jobs]   (local paths; syncs both to the builder)
G=$1; V=$2; F=$3; N=$4; J=${5:-2}
S=watchman2:rush2049/scratch/frontier/w36x4
T=/home/cburnes/projects/rush2049-decomp/cloud/work/frontier/w36x4/tools
rsync -aq $T/gscore.py $T/gb.py $S/tools/
rsync -aq --delete $G/ $S/cand/gb_base_$(basename $V)/
rsync -aq --delete $V/ $S/cand/gb_$(basename $V)/
ssh watchman2 "cd ~/rush2049/scratch/frontier/w36x4 && IDO_DIR=\$HOME/rush2049/cache/toolkits/796ae99a5cb7922e335e3afd87008753c573c82b45d04e8cfbce4f513cdbfbf5/ido nice python3 tools/gb.py cand/gb_base_$(basename $V) cand/gb_$(basename $V) $F $N --jobs $J"
scp -q $S/cand/gb_$(basename $V)/score.txt $V/score.txt
