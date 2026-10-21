#!/bin/bash
# usage: gd.sh FILE.c NAME [extra gscore args]   -> fn compile + aligned diff (full)
F=$1; N=$2; shift 2
B=$(basename $F)
rsync -aq /home/cburnes/projects/rush2049-decomp/cloud/work/frontier/w36w3/tools/gscore.py watchman2:rush2049/scratch/frontier/w36w3/tools/gscore.py
scp -q $F watchman2:rush2049/scratch/frontier/w36w3/cand/$B || exit 1
FL=$(head -1 $F | sed -n 's#^/\* flags: \(.*\) \*/#\1#p'); FL=${FL:-"-g0 -O3 -mips2 -G 0 -non_shared"}
ssh watchman2 "cd ~/rush2049/scratch/frontier/w36w3 && IDO_DIR=\$HOME/rush2049/cache/toolkits/796ae99a5cb7922e335e3afd87008753c573c82b45d04e8cfbce4f513cdbfbf5/ido timeout 300 python3 tools/gscore.py fn cand/$B $N --flags \"$FL\" --diff $N $* 2>&1"
