#!/bin/bash
# usage: sc.sh file.c NAME [flags]   -- copies file to builder cand/ and scores fn standalone
f=$1; n=$2; fl=${3:--O3}
S=rush2049/scratch/frontier/w35b
scp -q "$f" watchman2:$S/cand/$n.c
ssh watchman2 "cd $S && IDO_DIR=\$HOME/rush2049/cache/toolkits/796ae99a5cb7922e335e3afd87008753c573c82b45d04e8cfbce4f513cdbfbf5/ido python3 tools/cloud/score.py fn cand/$n.c $n --targets asm/us/ovl_a --flags \"-g0 $fl -mips2 -G 0 -non_shared\""
