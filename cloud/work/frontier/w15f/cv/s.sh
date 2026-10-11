#!/bin/sh
# s.sh FILE...: unit-score camera_victory (+ func_800C3AD0 neighbours) for each file; one line each
cd /home/cburnes/projects/rush2049-decomp
for f in "$@"; do
  r=$(python3 -m tools.conveyor.pipeline.blob_unit --tag w15f score camera_victory --with "$(realpath --relative-to=. cloud/work/frontier/w15f/cv/$f 2>/dev/null || echo $f)" $UARGS 2>&1 | grep -E "EQUAL|FAIL|rror" | head -1)
  t=$(TAG=w15f python3 cloud/work/frontier/tools/trace/udiff.py camera_victory --obj build/blob_unit/w15f/unit.o 2>&1 | tail -1)
  echo "$f: $r | $t"
done
