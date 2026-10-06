#!/bin/bash
# usage: run.sh file.c... -- score each; print words-diff, frame, s-reg use
T=/home/cburnes/projects/rush2049-decomp/cloud/work/frontier/w9c/tools
for f in "$@"; do
  out=$($T/sc.sh $(realpath $f) 2>&1)
  st=$(echo "$out" | grep -oE "FAIL func_8008E408: [0-9]+ of [0-9]+ words differ; compiled body is [0-9]+|[0-9]+ of [0-9]+ words differ|MATCH|EQUAL|equal.*" | head -1)
  fr=$(grep -m1 "addiu sp,sp" /tmp/claude-1000/udiff/u.txt)
  s=$(grep -cE "\bs[0-7]\b" /tmp/claude-1000/udiff/u.txt)
  nd=$(echo "$out" | tail -2 | tr "\n" " ")
  echo "$f | $st | $fr | sregs=$s | linediff=$nd"
done
