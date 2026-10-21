#!/bin/bash
# vb9058.sh FILE... : unit-score each D9058 variant sequentially; prints D9058/D91A0 verdicts + udiff summary
cd /home/cburnes/projects/rush2049-decomp
for f in "$@"; do
  echo "== $f"
  D9058=$f cloud/work/frontier/w36k/tools/uu.sh cloud/work/frontier/w36k/groups/controlscr/caller.c | grep -E "9058|91A0|816C|8154" | grep -v EQUAL
  TAG=w36k python3 cloud/work/frontier/tools/trace/udiff.py func_800D9058 --summary
done
