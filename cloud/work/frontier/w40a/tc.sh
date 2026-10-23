#!/bin/bash
# usage: tc.sh file.c [flags]  -> compile one file with IDO on the builder (cc -c), objdump -d on the Pi
F=$1; FL=${2:-"-g0 -O3 -mips2 -G 0 -non_shared"}
B=$(basename $F .c)
scp -q $F watchman2:rush2049/scratch/frontier/w40a/cand/$B.c
ssh watchman2 "cd ~/rush2049/scratch/frontier/w40a/cand && \$HOME/rush2049/cache/toolkits/796ae99a5cb7922e335e3afd87008753c573c82b45d04e8cfbce4f513cdbfbf5/ido/cc -c $FL -o $B.o $B.c" || exit 1
mkdir -p /tmp/claude-1000/w40a_obj
scp -q watchman2:rush2049/scratch/frontier/w40a/cand/$B.o /tmp/claude-1000/w40a_obj/$B.o
mips-linux-gnu-objdump -d -z --no-show-raw-insn /tmp/claude-1000/w40a_obj/$B.o | sed -n '/<.*>:/,$p'
