#!/bin/sh
# e4.sh FILE...: unit-score func_800E4B58 (+ E451C/E4300/E398C internal) with each group file; one line each
cd /home/cburnes/projects/rush2049-decomp
for f in "$@"; do
  r=$(python3 -m tools.conveyor.pipeline.blob_unit --tag w16d score func_800E4B58 func_800E451C func_800E398C --with "$f" --internal func_800E451C --internal func_800E4300 --internal func_800E398C 2>&1 | grep -E "(EQUAL|FAIL) func_800E4B58|(EQUAL|FAIL) func_800E451C|rror" | sed 's/^ *//' | cut -c1-90 | paste -sd' ')
  w=$(TAG=w16d python3 cloud/work/frontier/tools/trace/udiff.py func_800E4B58 --summary 2>&1 | tail -1); o=$(TAG=w16d python3 cloud/work/frontier/tools/trace/udiff.py func_800E4B58 --summary --ops 2>&1 | tail -1 | sed 's/.*rows \([0-9]*\).*/\1/')
  echo "$(basename $f): $r | $w | ops $o"
done
