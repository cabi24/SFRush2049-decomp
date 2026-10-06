#!/bin/sh
# run.sh FN cand.c : score FN in the w9b blob_unit and print a short diff summary
cd /home/cburnes/projects/rush2049-decomp
python3 -m tools.conveyor.pipeline.blob_unit --tag w9b score "$1" --with "$2" 2>&1 | grep -E "EQUAL|FAIL|equal|inlined"
python3 cloud/work/frontier/w9b/udiff.py "$1" | grep -E "sp,sp,|^want"
