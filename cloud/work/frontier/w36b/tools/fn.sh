#!/bin/bash
# usage: fn.sh SRC NAME [img]  -- standalone score on builder
src=$1; name=$2; img=${3:-ovl_a}
S=rush2049/scratch/frontier/w36b
scp -q $src watchman2:$S/cand/$name.c && \
ssh watchman2 "cd $S && IDO_DIR=\$HOME/rush2049/cache/toolkits/796ae99a5cb7922e335e3afd87008753c573c82b45d04e8cfbce4f513cdbfbf5/ido python3 tools/cloud/score.py fn cand/$name.c $name --targets asm/us/$img --flags '-g0 -O3 -mips2 -G 0 -non_shared'" 2>&1 | cut -c1-250 | head -${4:-12}
