#!/bin/bash
# usage: cmps.sh FILE.c VRAM SIZE [cflags...] : IDO 5.3 cc -S (cflags, default -g0 -O2 -mips2), drop ".option O2", assemble with as1 -O1; instruction diff vs ROM
F=$1; V=$2; Z=$3; shift 3
FL="${*:--g0 -O2 -mips2}"
S=/tmp/claude-1000/-home-cburnes-projects-rush2049-decomp/e6d4d7af-aa0e-4f5a-98a8-2e25f3b6dd6b/scratchpad/w25a
B=$(basename $F .c)_s$$
scp -q $F watchman2:rush2049/scratch/frontier/w25a/cmp_$B.c
ssh -n watchman2 "cd ~/rush2049/scratch/frontier/w25a && CC=~/rush2049/cache/toolkits/796ae99a5cb7922e335e3afd87008753c573c82b45d04e8cfbce4f513cdbfbf5/ido/cc && CS=${CCS:-\$CC} && R=repo && \$CS -S $FL -G 0 -non_shared -Xcpluscomm -Wab,-r4300_mul -D_LANGUAGE_C -I\$R/include -I\$R/include/PR -I\$R/src/rom -o cmp_$B.s cmp_$B.c 2>&1 | grep -iv 'warning' | head -5; sed '/\.option\tO[0-9]/d' cmp_$B.s > cmp_${B}n.s && \$CC -c -O1 -mips2 -G 0 -non_shared -Wab,-r4300_mul -o cmp_$B.o cmp_${B}n.s 2>&1 | head -3"
scp -q watchman2:rush2049/scratch/frontier/w25a/cmp_$B.o $S/cmp/$B.o
python3 $S/sbs.py $S/cmp/$B.o $V $Z ${VERBOSE:+v}
