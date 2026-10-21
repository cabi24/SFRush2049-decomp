#!/bin/bash
# usage: c1.sh FILE.c [FLAGS]   compile on the builder with IDO (single file) and objdump -dr
F=$1; shift
FLAGS=${*:--g0 -O3 -mips2 -G 0 -non_shared}
B=$(basename $F .c)
scp -q $F watchman2:rush2049/scratch/frontier/w35f/cand/_c1_$B.c
ssh watchman2 "cd ~/rush2049/scratch/frontier/w35f/cand && IDO=\$HOME/rush2049/cache/toolkits/796ae99a5cb7922e335e3afd87008753c573c82b45d04e8cfbce4f513cdbfbf5/ido && \$IDO/cc -c $FLAGS -Wab,-r4300_mul -o _c1_$B.o _c1_$B.c 2>&1 | head -20 && mips-linux-gnu-objdump -dr -M gpr-names=32 _c1_$B.o | grep -v '^$' | sed -n '7,\$p'"
