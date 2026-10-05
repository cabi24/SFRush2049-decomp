#!/bin/sh
# usage: f.sh file.c [lines] [extra full.py args]  -- -O3 group build (keep render_large_objects), aligned diff vs retail; full diff in scratchpad/out.txt
f="$1"; l="${2:-60}"; shift; shift
O=/tmp/claude-1000/-home-cburnes-projects-rush2049-decomp/80274e1e-9a2b-4e9a-b2ec-780e794c33c9/scratchpad/out.txt
scp -q "$f" watchman2:rush2049/scratch/frontier/w2j/cand/_f.c && ssh watchman2 "cd ~/rush2049/scratch/frontier/w2j && IDO_DIR=\$HOME/rush2049/cache/toolkits/796ae99a5cb7922e335e3afd87008753c573c82b45d04e8cfbce4f513cdbfbf5/ido python3 cand/full.py cand/_f.c render_large_objects --flags '-g0 -O3 -mips2 -G 0 -non_shared' --keep render_large_objects $*" > $O 2>&1
tail -1 $O
head -$l $O
