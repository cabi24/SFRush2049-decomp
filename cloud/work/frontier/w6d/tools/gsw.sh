#!/bin/sh
# usage: gsw.sh GROUPDIR NAME VARIANT.c... -- score variant group.c files (copied into a temp group dir)
g=$1; n=$2; shift; shift
for v in "$@"; do
  d=$(mktemp -d); cp $g/group.json $d/; cp $v $d/group.c
  r=$("$(dirname $0)/gfd.sh" $d $n | tail -1)
  echo "$v: $r"; rm -rf $d
done
