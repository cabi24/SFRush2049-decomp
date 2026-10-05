#!/bin/sh
# fframe.sh VARIANT.c -- func_8009F058 frame offsets for a part_f058 variant
F=/tmp/claude-1000/-home-cburnes-projects-rush2049-decomp/80274e1e-9a2b-4e9a-b2ec-780e794c33c9/scratchpad/ff.txt
./var.sh part_f058.c $1 func_8009F058 --all > $F
echo "frame $(grep -m1 'addiu sp,sp' $F | awk '{print $NF}') | perspNorm: $(grep -m1 'addiu a1,sp' $F | awk '{print $NF}') | uv: $(grep -m1 'addiu s0,sp' $F | awk '{print $NF}') | 376: $(grep -m1 'sw t8,376\|,3[0-9][0-9](sp)' $F | awk '{print $3, $NF}') | p: $(grep -m1 'addiu a1,sp,2' $F | awk '{print $NF}') | $(tail -1 $F)"
