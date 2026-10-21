#!/bin/bash
# usage: sfile.sh SRC OUT.s  -- compile -S (ugen output before as1 scheduling? no: -S emits as1 input listing)
src=$1; out=$2
S=rush2049/scratch/frontier/w36b
scp -q $src watchman2:$S/cand/sfile.c && \
ssh watchman2 "cd $S/cand && \$HOME/rush2049/cache/toolkits/796ae99a5cb7922e335e3afd87008753c573c82b45d04e8cfbce4f513cdbfbf5/ido/cc -S -g0 -O3 -mips2 -G 0 -non_shared sfile.c -o sfile.s 2>&1 | head; cat sfile.s" > $out
