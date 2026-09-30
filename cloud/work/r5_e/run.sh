#!/bin/bash
# usage: run.sh file fn [flags]
cd /home/user/SFRush2049-decomp
python3 tools/cloud/score.py fn "$1" "$2" --flags "${3:--g0 -O2 -mips2 -G 0 -non_shared}" 2>&1 | grep -E "MATCH|differ|extra" | tail -2
