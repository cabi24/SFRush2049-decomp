#!/bin/bash
# usage: cc.sh FILE.c [flags] -> compile on builder, print objdump -d
cd /home/cburnes/projects/rush2049-decomp || exit 1
L=cloud/work/frontier/w36c
B=watchman2:rush2049/scratch/frontier/w36c
scp -q $L/$1 $B/lane_cc.c || exit 1
FL="${2:--g0 -O3 -mips2 -G 0 -non_shared}"
ssh watchman2 "cd ~/rush2049/scratch/frontier/w36c && IDO_DIR=\$HOME/rush2049/cache/toolkits/796ae99a5cb7922e335e3afd87008753c573c82b45d04e8cfbce4f513cdbfbf5/ido python3 -c \"
import sys; sys.path.insert(0,'tools/cloud')
import score
score.compile_single('lane_cc.c', '$FL', 'lane_cc.o')
\"" && scp -q $B/lane_cc.o /tmp/claude-1000/-home-cburnes-projects-rush2049-decomp/268d487e-6278-4348-8a37-862b514a0019/scratchpad/cc.o && mips-linux-gnu-objdump -d -r -M gpr-names=32 /tmp/claude-1000/-home-cburnes-projects-rush2049-decomp/268d487e-6278-4348-8a37-862b514a0019/scratchpad/cc.o | sed -n '7,$p'
