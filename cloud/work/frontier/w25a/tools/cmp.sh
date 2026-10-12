#!/bin/bash
# usage: cmp.sh FILE.c VRAM [ido53|ido71] [flags...]   -> compiles on builder (w25a scratch repo includes), compares .text with ROM (w24t compare.py)
set -e
F=$1; V=$2; C=${3:-ido53}; shift 3 || true
FL="${*:--g0 -O2 -mips2}"
S=/tmp/claude-1000/-home-cburnes-projects-rush2049-decomp/e6d4d7af-aa0e-4f5a-98a8-2e25f3b6dd6b/scratchpad/w25a/cmp
W=/home/cburnes/projects/rush2049-decomp/.claude/worktrees/w25a
B=$(basename $F .c)_$$
scp -q $F watchman2:rush2049/scratch/frontier/w25a/cmp_$B.c
if [ $C = ido71 ]; then CC='~/rush2049/repo/tools/ido-static-recomp/build71/out/cc'; else CC='~/rush2049/cache/toolkits/796ae99a5cb7922e335e3afd87008753c573c82b45d04e8cfbce4f513cdbfbf5/ido/cc'; fi
ssh -n watchman2 "cd ~/rush2049/scratch/frontier/w25a && R=repo && $CC -c $FL -G 0 -non_shared -Xcpluscomm -Wab,-r4300_mul -D_LANGUAGE_C -I\$R/include -I\$R/include/PR -I\$R/src/rom -o cmp_$B.o cmp_$B.c 2>&1 | grep -v 'warning\|^$' | head -5; true"
scp -q watchman2:rush2049/scratch/frontier/w25a/cmp_$B.o $S/$B.o
python3 $W/cloud/work/frontier/w24t/rename/compare.py $S/$B.o $V $W/baserom.us.z64 > $S/$B.log 2>&1 || true
grep RESULT $S/$B.log || tail -3 $S/$B.log
