#!/bin/bash
# usage: try.sh file fn [flags-opt]
cd /home/user/SFRush2049-decomp
python3 tools/cloud/score.py fn "$1" "$2" --flags "${3:--g0 -O2 -mips2 -G 0 -non_shared}" 2>&1 | tail -${4:-12}
