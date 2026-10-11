#!/bin/sh
# sc.sh FILE...: unit-score E4B58 (+E451C, E398C internal) per file: verdict lines + udiff word/ops rows
F0=""; for x in "$@"; do F0="$F0 $(realpath $x)"; done; set -- $F0
cd /home/cburnes/projects/rush2049-decomp
for f in "$@"; do
  r=$(python3 -m tools.conveyor.pipeline.blob_unit --tag w17b score func_800E4B58 func_800E451C func_800E398C --with "$f" --internal func_800E451C --internal func_800E4300 --internal func_800E398C 2>&1 | grep -E "(EQUAL|FAIL) func_800E|rror" | sed 's/^ *//' | cut -c1-70 | paste -sd' ')
  w=$(TAG=w17b python3 cloud/work/frontier/tools/trace/udiff.py func_800E4B58 --summary 2>&1 | tail -1 | sed 's/want.*got [0-9]*; //'); o=$(TAG=w17b python3 cloud/work/frontier/tools/trace/udiff.py func_800E4B58 --summary --ops 2>&1 | tail -1 | sed 's/.*rows \([0-9]*\).*/\1/')
  echo "$(basename $f): $r | $w | ops $o"
done
