#!/bin/sh
# usage: objget.sh cand.c NAME out.o -- compile at -O3 on the builder (score.compile_single) and fetch the object
scp -q "$1" watchman2:rush2049/scratch/frontier/w2b/cand/_og_$2.c && ssh watchman2 "cd ~/rush2049/scratch/frontier/w2b && IDO_DIR=\$HOME/rush2049/cache/toolkits/796ae99a5cb7922e335e3afd87008753c573c82b45d04e8cfbce4f513cdbfbf5/ido python3 -c \"
import sys; sys.path.insert(0,'tools/cloud'); import score
from pathlib import Path
score.compile_single('cand/_og_$2.c','-g0 -O3 -mips2 -G 0 -non_shared',Path('cand/_og_$2.o'))\"" && scp -q watchman2:rush2049/scratch/frontier/w2b/cand/_og_$2.o "$3"
