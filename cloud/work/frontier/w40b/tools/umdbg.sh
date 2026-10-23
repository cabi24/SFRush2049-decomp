#!/bin/sh
# usage: umdbg.sh OUT.log  -> rerun umerge -debug (lane-instrumented copy, tools/patch_umerge.py: UMK knapsack lines on stderr)
# on the last w40b unit stage (copy of split/st in the lane scratch); checks the merged output equals the stage's (fidelity)
# and fetches the log.
O=${1:-/tmp/claude-1000/umdbg.log}
ssh watchman2 'S=~/rush2049/scratch/frontier/unit/w40b/stage; D=~/rush2049/scratch/frontier/w40b/um; rm -rf $D; mkdir -p $D; cp $S/split $S/st $D/ && cd $D && ~/rush2049/scratch/frontier/w40b/umsrc/umerge -v -debug -Olimit 5000 -mips2 -EB -g0 -O3 split -o merged -t st > um.log 2>&1; echo rc=$?; cmp merged $S/merged && echo FIDELITY_OK' && scp -q watchman2:rush2049/scratch/frontier/w40b/um/um.log "$O" && wc -l "$O"
