#!/bin/sh
# usage: um.sh cand.c FN [internal helper names...] -- score FN in the unit with helpers internal; print diff rows
cd /home/cburnes/projects/rush2049-decomp
src="$1"; fn="$2"; shift; shift
ints=""; for h in "$@"; do ints="$ints --internal $h"; done
python3 -m tools.conveyor.pipeline.blob_unit --tag w2d score $fn $ints --with "$src" 2>&1 | grep -v "^       +0x" | tail -5
grep -A14 "^  $fn\$" build/blob_unit/w2d/umerge.log | grep inlining | sed 's/inlining//' | tr -s '\n ' ' '; echo
python3 cloud/work/frontier/w2d/objdiff.py build/blob_unit/w2d/unit.o $fn
