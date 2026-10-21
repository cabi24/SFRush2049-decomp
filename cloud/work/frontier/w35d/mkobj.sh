#!/bin/bash
# usage: mkobj.sh groups/<dir>  -> builds object on builder, copies to /tmp scratch ($OUT or scratchpad)
cd /home/cburnes/projects/rush2049-decomp || exit 1
L=cloud/work/frontier/w35d
B=watchman2:rush2049/scratch/frontier/w35d
rsync -aq --exclude 'v/' --exclude '*.pyc' $L/ $B/lane/ || exit 1
OUT=${OUT:-/tmp/claude-1000/-home-cburnes-projects-rush2049-decomp/4b393cd0-4116-4767-90e0-4ddf67d9ff98/scratchpad/out.o}
ssh watchman2 "cd ~/rush2049/scratch/frontier/w35d && IDO_DIR=\$HOME/rush2049/cache/toolkits/796ae99a5cb7922e335e3afd87008753c573c82b45d04e8cfbce4f513cdbfbf5/ido python3 -c \"
import sys; sys.path.insert(0,'tools/cloud')
import score
from pathlib import Path
score.compile_group(Path('lane/$1'), 'lane_out.o')
\"" && scp -q watchman2:rush2049/scratch/frontier/w35d/lane_out.o $OUT && mips-linux-gnu-readelf -s $OUT | grep -i "FUNC\|OBJECT" | head -40
