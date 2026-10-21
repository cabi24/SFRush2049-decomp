#!/bin/sh
# usage: many.sh NAME file1.c file2.c ...   -> summary (words, ops) per file
name="$1"; shift
for f in "$@"; do
  w=$(/home/cburnes/projects/rush2049-decomp/cloud/work/frontier/w36v/tools/sd.sh "$f" "$name" --summary 2>&1 | tail -1)
  b=$(basename "$f" .c)
  o=$(python3 /home/cburnes/projects/rush2049-decomp/cloud/work/frontier/tools/trace/udiff.py "$name" --obj /tmp/claude-1000/-home-cburnes-projects-rush2049-decomp/268d487e-6278-4348-8a37-862b514a0019/scratchpad/sd/$b.o --ops --summary 2>&1 | tail -1 | sed 's/.*differing rows \([0-9]*\).*/\1/')
  echo "$b: $w | ops $o"
done
