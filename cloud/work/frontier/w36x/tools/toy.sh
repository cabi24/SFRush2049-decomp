#!/bin/bash
# toy.sh FILE.c [flags] : compile on builder and objdump
f=$1; b=$(basename $f .c)
S=rush2049/scratch/frontier/w36x
scp -q $f watchman2:$S/cand/$b.c && ssh watchman2 "cd $S/cand && \$HOME/rush2049/cache/toolkits/796ae99a5cb7922e335e3afd87008753c573c82b45d04e8cfbce4f513cdbfbf5/ido/cc -c ${2:--g0 -O3 -mips2 -G 0 -non_shared} -Wab,-r4300_mul $b.c -o $b.o 2>&1 | grep -v Warning | head -5; mips-linux-gnu-objdump -d -z -M gpr-names=32 $b.o 2>/dev/null || objdump -d $b.o" | grep -E "^\s+[0-9a-f]+:" | sed 's/^ *[0-9a-f]*:\s*[0-9a-f]* *//'
