#!/bin/bash
# usage: dump.sh FILE.c [flags...]  -> compiled .text disassembly on stdout (builder w36j)
F=$1; shift
FL=${*:-"-g0 -O3 -mips2 -G 0 -non_shared"}
B=$(basename $F .c)
scp -q $F watchman2:rush2049/scratch/frontier/w36j/cand/_d_$B.c
ssh watchman2 "cd ~/rush2049/scratch/frontier/w36j/cand && TK=\$HOME/rush2049/cache/toolkits/796ae99a5cb7922e335e3afd87008753c573c82b45d04e8cfbce4f513cdbfbf5 && \$TK/ido/cc -c $FL -Wab,-r4300_mul -o _d_$B.o _d_$B.c && mips-linux-gnu-objdump -dr -M gpr-names=32 _d_$B.o"
