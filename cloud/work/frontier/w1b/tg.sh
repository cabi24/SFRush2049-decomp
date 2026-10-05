#!/bin/sh
# usage: tg.sh file.c NAME [lines] [keep]  -- aligned diff in -O3 group mode (uld -kp, -Olimit 5000)
f="$1"; n="$2"; l="${3:-60}"; k="${4:-$2}"
O=/tmp/claude-1000/-home-cburnes-projects-rush2049-decomp/80274e1e-9a2b-4e9a-b2ec-780e794c33c9/scratchpad/w1b_out.txt
./full.sh "$f" "$n" --flags "\"-g0 -O3 -mips2 -G 0 -non_shared\"" --keep "$k" > $O 2>&1
grep -c '^|' $O
grep -v '^ ' $O | head -$l
