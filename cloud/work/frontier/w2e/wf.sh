#!/bin/sh
# usage: wf.sh cand.c NAME -- strict tail + aligned diff saved to scratch, prints summary
S=/tmp/claude-1000/-home-cburnes-projects-rush2049-decomp/80274e1e-9a2b-4e9a-b2ec-780e794c33c9/scratchpad
d=$(dirname "$0")
$d/full.sh "$1" "$2" --flags '"-g0 -O3 -mips2 -G 0 -non_shared"' > $S/last_diff.txt 2>&1
tail -1 $S/last_diff.txt
