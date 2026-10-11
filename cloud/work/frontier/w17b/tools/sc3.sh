#!/bin/sh
# sc3.sh FILE...: unit-score the four (E4B58 kept; E451C/E4300/E398C internal); one line each incl E398C rows
F0=""; for x in "$@"; do F0="$F0 $(realpath $x)"; done; set -- $F0
cd /home/cburnes/projects/rush2049-decomp
for f in "$@"; do
  r=$(python3 -m tools.conveyor.pipeline.blob_unit --tag w17b score func_800E4B58 func_800E451C func_800E4300 func_800E398C --with "$f" --internal func_800E451C --internal func_800E4300 --internal func_800E398C 2>&1 | grep -E "(EQUAL|FAIL) func_800E|rror" | sed 's/^ *//' | cut -c1-58 | paste -sd' ')
  w=$(TAG=w17b python3 cloud/work/frontier/tools/trace/udiff.py func_800E398C --summary 2>&1 | tail -1 | sed 's/want.*got \([0-9]*\); differing rows \([0-9]*\) (words); frame \([0-9/]*\).*/got \1 rows \2 frame \3/'); o=$(TAG=w17b python3 cloud/work/frontier/tools/trace/udiff.py func_800E398C --summary --ops 2>&1 | tail -1 | sed 's/.*rows \([0-9]*\).*/\1/')
  echo "$(basename $f): $r | E398C $w ops $o"
done
