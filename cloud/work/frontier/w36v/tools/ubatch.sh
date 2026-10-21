#!/bin/sh
# usage: ubatch.sh NAME DIR "blob_unit args"   -- unit-score every DIR/v*.c, print words/ops rows, sorted
name="$1"; dir="$2"; args="$3"
cd /home/cburnes/projects/rush2049-decomp
for f in "$dir"/*.c; do
  python3 -m tools.conveyor.pipeline.blob_unit --tag ${UTAG:-w36v} score $name --with "$f" $args > /tmp/claude-1000/-home-cburnes-projects-rush2049-decomp/268d487e-6278-4348-8a37-862b514a0019/scratchpad/ub.log 2>&1
  st=$(grep -E "EQUAL|FAIL" /tmp/claude-1000/-home-cburnes-projects-rush2049-decomp/268d487e-6278-4348-8a37-862b514a0019/scratchpad/ub.log | head -1 | cut -c1-60)
  w=$(TAG=${UTAG:-w36v} python3 cloud/work/frontier/tools/trace/udiff.py $name --summary 2>/dev/null | sed 's/.*differing rows \([0-9]*\).*frame \([0-9/]*\).*/\1 \2/')
  o=$(TAG=${UTAG:-w36v} python3 cloud/work/frontier/tools/trace/udiff.py $name --ops --summary 2>/dev/null | sed 's/.*differing rows \([0-9]*\).*/\1/')
  echo "$(basename $f) words=$w ops=$o | $st"
done | sort -t= -k2 -n
