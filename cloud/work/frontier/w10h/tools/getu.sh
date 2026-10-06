#!/bin/bash
# usage: getu.sh LABEL -- fetch merged+opt ucode of the last w10h unit build and dump func_8008E408 (constants filter)
S=/tmp/claude-1000/-home-cburnes-projects-rush2049-decomp/80274e1e-9a2b-4e9a-b2ec-780e794c33c9/scratchpad/u; mkdir -p $S
for k in merged opt; do scp -q watchman2:rush2049/scratch/frontier/unit/w10h/stage/$k $S/${k}_$1; python3 /home/cburnes/projects/rush2049-decomp/cloud/work/frontier/w10h/tools/udump.py $S/${k}_$1 ${FILT:-0x40000,0x800,0x400,0x200} > $S/$1.$k.ud; done
