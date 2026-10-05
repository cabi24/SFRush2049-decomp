#!/bin/sh
# frame.sh VARIANT.c -- particle_system frame/array offsets for a part_ps variant
./var.sh part_ps.c $1 particle_system --all > /tmp/claude-1000/-home-cburnes-projects-rush2049-decomp/80274e1e-9a2b-4e9a-b2ec-780e794c33c9/scratchpad/fr.txt
F=/tmp/claude-1000/-home-cburnes-projects-rush2049-decomp/80274e1e-9a2b-4e9a-b2ec-780e794c33c9/scratchpad/fr.txt
echo "frame: $(grep -m1 'addiu sp,sp' $F | awk '{print $NF}')  gfx: $(sed -n 7p $F | awk '{print $NF}')  rect(x0): $(grep 'sh t8' $F | head -1 | awk '{print $NF}') d0: $(grep -m1 'lwc1 \$f2,' $F | awk '{print $NF}')  dir: $(grep -m1 'addiu a0,sp' $F | awk '{print $NF}')  rows: $(tail -1 $F)"
