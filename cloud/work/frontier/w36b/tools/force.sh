#!/bin/bash
# force.sh LABEL NAME SPEC  -- colouring oracle on snapshot, diff against ovl_a
cd /home/cburnes/projects/rush2049-decomp
export TAG=w36b SCR=rush2049/scratch/frontier/w36b
cloud/work/frontier/tools/trace/force.sh $1 $2 "$3" 2>&1 | grep -E "forced|declined" | head -20
python3 cloud/work/frontier/w36b/tools/dd.py x $2 --obj build/trace/w36b/f_$1.o ${4:---norm} 2>/dev/null | head -${5:-3}
