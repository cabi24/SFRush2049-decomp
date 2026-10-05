#!/bin/sh
# usage: dis.sh cand.c NAME  -- full disassembly of the candidate (builder, agentB copy)
scp -q "$1" watchman2:rush2049/scratch/frontier/agentB/cand/$2.c
ssh -o BatchMode=yes watchman2 "cd ~/rush2049/scratch/frontier/agentB && IDO=\$HOME/rush2049/cache/toolkits/796ae99a5cb7922e335e3afd87008753c573c82b45d04e8cfbce4f513cdbfbf5/ido && \$IDO/cc -c -g0 -O2 -mips2 -G 0 -non_shared -Wab,-r4300_mul -o cand/$2.o cand/$2.c && mips-linux-gnu-objdump -d -r -M reg-names=32 cand/$2.o | sed -n '/<$2>:/,/^\$/p' | cut -f3-"
