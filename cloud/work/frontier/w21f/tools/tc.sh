#!/bin/sh
# tc.sh FILE.c LABEL: unit build + snapshot + SAVELOG oracle pass (no forcing); prints strict line, the spill homes
# and the first compute_save records of the webs w21f tracks: slot (tot 166), type (tot 192), model (tot 32, nocs>40),
# const 5 (tot 30, nocs>40), const 0/68 (tot 150). Diagnostic only.
f=$(realpath $1); L=$2
D=/tmp/claude-1000/-home-cburnes-projects-rush2049-decomp/e6d4d7af-aa0e-4f5a-98a8-2e25f3b6dd6b/scratchpad
cd /home/cburnes/projects/rush2049-decomp; export TAG=w21f; T=cloud/work/frontier/tools/trace
$T/us.sh entity_spawn_init $f | cut -c1-130; $T/snap.sh $L >/dev/null 2>&1; $T/spill.sh $L entity_spawn_init >/dev/null 2>&1
ssh watchman2 "cd ~/rush2049/scratch/frontier/w21f/st_$L && grep 'proc=entity_spawn_init' spill.log | grep -E '^(HOME|AREA f_r)' | sed 's/proc=entity_spawn_init //;s/ size=4//;s/AREA f_readnxtinst:93590 //' | head -6 | tr '\n' ';'"; echo
sh cloud/work/frontier/w21f/tools/oracle.sh $L "SAVELOG=1" --summary >/dev/null
scp -q watchman2:rush2049/scratch/frontier/w21f/st_$L/or.spl $D/$L.spl
grep -E '^\[SAVE\] lr=[0-9]+ card=' $D/$L.spl | awk '!seen[$2]++' | awk '{split($4,n,"=");split($5,t,"="); if (n[2]>=40 && (t[2]==166||t[2]==192||t[2]==32||t[2]==30||t[2]==150||t[2]==662||t[2]==661)) printf "%s %s %s %s | ", $2,$3,$4,$5}'; echo
