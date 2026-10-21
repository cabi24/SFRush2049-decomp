#!/bin/bash
# usage: mini.sh file.c [flags] -> compile on builder (-O3 default), print disassembly
f=$1; fl=${2:--O3}
S=rush2049/scratch/frontier/w35b
scp -q $f watchman2:$S/cand/mini.c
ssh watchman2 "cd $S && \$HOME/rush2049/cache/toolkits/796ae99a5cb7922e335e3afd87008753c573c82b45d04e8cfbce4f513cdbfbf5/ido/cc -c -g0 $fl -mips2 -G 0 -non_shared -Wab,-r4300_mul -o cand/mini.o cand/mini.c && cat cand/mini.o" > /tmp/claude-1000/-home-cburnes-projects-rush2049-decomp/4b393cd0-4116-4767-90e0-4ddf67d9ff98/scratchpad/mini.o
mips-linux-gnu-objdump -dr -M gpr-names=32 /tmp/claude-1000/-home-cburnes-projects-rush2049-decomp/4b393cd0-4116-4767-90e0-4ddf67d9ff98/scratchpad/mini.o | grep -v "^$" | sed -n 6,200p
