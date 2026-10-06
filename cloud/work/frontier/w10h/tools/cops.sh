#!/bin/bash
# usage: cops.sh [OBJ] -- register-normalised opcode diff of camera_play_script (last w10h unit or given obj); full list in scratch/ops.txt
R=/home/cburnes/projects/rush2049-decomp; S=/tmp/claude-1000/-home-cburnes-projects-rush2049-decomp/80274e1e-9a2b-4e9a-b2ec-780e794c33c9/scratchpad
O=${1:-$R/build/blob_unit/w10h/unit.o}
cd $R && python3 cloud/work/frontier/w3a/tools/udiff.py ${FN:-camera_play_script} --obj $O --all > $S/ud.txt
python3 $R/cloud/work/frontier/w10h/tools/ops.py $S/ud.txt > $S/ops.txt
