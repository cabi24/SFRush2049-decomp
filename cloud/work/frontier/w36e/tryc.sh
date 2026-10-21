#!/bin/bash
# usage: tryc.sh file.c [flags] -> compile a single file on the builder, print objdump -dr
cd /home/cburnes/projects/rush2049-decomp || exit 1
SP=/tmp/claude-1000/-home-cburnes-projects-rush2049-decomp/268d487e-6278-4348-8a37-862b514a0019/scratchpad/w36e
FL="${2:--g0 -O3 -mips2 -G 0 -non_shared}"
scp -q "$1" watchman2:rush2049/scratch/frontier/w36e/lane_try.c || exit 1
ssh watchman2 "cd ~/rush2049/scratch/frontier/w36e && IDO_DIR=\$HOME/rush2049/cache/toolkits/796ae99a5cb7922e335e3afd87008753c573c82b45d04e8cfbce4f513cdbfbf5/ido python3 -c \"
import sys; sys.path.insert(0,'tools/cloud')
import score
score.compile_single('lane_try.c', '$FL', 'lane_try.o')
\"" && scp -q watchman2:rush2049/scratch/frontier/w36e/lane_try.o $SP/try.o && mips-linux-gnu-objdump -dr --no-show-raw-insn $SP/try.o | sed -n 1,200p
