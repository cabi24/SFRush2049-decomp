#!/bin/sh
# lay.sh FILE.c: unit-score then print control_settings' merged-ucode Mmt layout (offsets from frame top)
D=/home/cburnes/projects/rush2049-decomp/cloud/work/frontier/w33d/control_settings
S=/tmp/claude-1000/-home-cburnes-projects-rush2049-decomp/e6d4d7af-aa0e-4f5a-98a8-2e25f3b6dd6b/scratchpad
$D/run.sh "$1" | tail -1
scp -q watchman2:rush2049/scratch/frontier/unit/w33d/stage/merged $S/merged_lay
python3 /home/cburnes/projects/rush2049-decomp/cloud/work/frontier/w12h/tools/udump.py $S/merged_lay 2080636926,37,476 > $S/lay.u 2>&1
python3 $D/mmt.py $S/lay.u | awk '$1 > -460' | tr -s ' ' | paste -sd ';'
