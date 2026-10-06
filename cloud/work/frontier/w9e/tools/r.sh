#!/bin/sh
# usage: r.sh FN CAND.c [extra blob_unit score args...]  -- score in unit (tag w9e) + aligned diff summary
fn=$1; c=$(realpath "$2"); shift 2
cd /home/cburnes/projects/rush2049-decomp
TAG=${TAG:-w9e}
python3 -m tools.conveyor.pipeline.blob_unit --tag $TAG score $fn "$@" --with "$c" 2>&1 | grep -E "EQUAL|FAIL|MATCH|rror|equal|inlined" | head -4
python3 cloud/work/frontier/w9e/tools/udiff.py $fn --obj build/blob_unit/$TAG/unit.o ${DIFF:-} | tail -${N:-1}
