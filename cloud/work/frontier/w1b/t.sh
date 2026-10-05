#!/bin/sh
# usage: t.sh file.c NAME [O2|O3] [lines]  -- aligned diff + count
f="$1"; n="$2"; o="${3:-O3}"; l="${4:-60}"
./full.sh "$f" "$n" --flags "\"-g0 -$o -mips2 -G 0 -non_shared\"" > /tmp/claude-1000/-home-cburnes-projects-rush2049-decomp/80274e1e-9a2b-4e9a-b2ec-780e794c33c9/scratchpad/out.txt 2>&1
grep -c '^|' /tmp/claude-1000/-home-cburnes-projects-rush2049-decomp/80274e1e-9a2b-4e9a-b2ec-780e794c33c9/scratchpad/out.txt
head -$l /tmp/claude-1000/-home-cburnes-projects-rush2049-decomp/80274e1e-9a2b-4e9a-b2ec-780e794c33c9/scratchpad/out.txt
