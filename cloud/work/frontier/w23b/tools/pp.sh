#!/bin/sh
# w23b (Pi): preprocess lane-relative SRC's FN reduced TU on the builder -> local OUT (for trace snap.sh)
src=$1; fn=$2; out=$3
L=/home/cburnes/projects/rush2049-decomp/cloud/work/frontier/w23b
rsync -aR -q $L/./$src watchman2:rush2049/scratch/frontier/w23b/lane/
ssh watchman2 "cd ~/rush2049/scratch/frontier/w23b && TK=\$HOME/rush2049/cache/toolkits/796ae99a5cb7922e335e3afd87008753c573c82b45d04e8cfbce4f513cdbfbf5 && python3 -c \"
import sys; sys.path[:0]=['.']
from tools.conveyor.seeds import context as c
open('lane/_pp.c','w').write(c.reduced_tu(open('lane/$src').read(),'$fn'))\" && \$TK/ido/cc -E -I include -I include/PR -I \$TK/shim lane/_pp.c | grep -v '^#'" > $out
