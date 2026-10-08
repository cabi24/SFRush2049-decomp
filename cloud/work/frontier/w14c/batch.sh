#!/bin/bash
# usage: batch.sh FN file1.c file2.c ... -> one summary line per file (-O3)
FN=$1; shift; S=rush2049/scratch/frontier/w14c
for f in "$@"; do b=$(basename "$f" .c); scp -q "$f" watchman2:$S/cand/$FN.$b.c; done
names=""; for f in "$@"; do b=$(basename "$f" .c); names="$names $FN.$b"; done
timeout 115 ssh watchman2 "cd ~/$S && for n in $names; do r=\$(IDO_DIR=\$HOME/rush2049/cache/toolkits/796ae99a5cb7922e335e3afd87008753c573c82b45d04e8cfbce4f513cdbfbf5/ido timeout 100 python3 tools/cloud/score.py fn cand/\$n.c $FN --flags '-g0 -O3 -mips2 -G 0 -non_shared' 2>&1 | tail -1); echo \$n: \$r; done"
