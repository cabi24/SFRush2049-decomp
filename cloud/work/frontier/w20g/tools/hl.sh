#!/bin/sh
# hl.sh FILE.c LABEL: unit build FILE, snapshot as LABEL, print the spill-home records (f_spilltemps) of entity_spawn_init
f=$(realpath $1); L=$2
cd /home/cburnes/projects/rush2049-decomp; export TAG=w20g; T=cloud/work/frontier/tools/trace
$T/us.sh entity_spawn_init $f | cut -c1-120; $T/snap.sh $L >/dev/null 2>&1; $T/spill.sh $L entity_spawn_init >/dev/null 2>&1
ssh watchman2 "cd ~/rush2049/scratch/frontier/w20g/st_$L && grep 'proc=entity_spawn_init' spill.log | grep -E '^(HOME|AREA f_)' | sed 's/proc=entity_spawn_init //' | tr '\n' ';'"; echo
