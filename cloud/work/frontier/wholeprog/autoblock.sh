#!/bin/sh
# usage: wp/autoblock.sh TAG [wp.py build options]
# Rebuild until no call that retail makes by jal is inlined by umerge:
# each offending callee gets a dead-code inline blocker (wp.py --block).
tag=$1; shift
list=wp/block_$tag.txt
: > $list
prev=-1
for i in 1 2 3 4 5 6; do
  wp/runall.sh "$tag" "$@" --block $list
  python3 wp/inline_edges.py "$tag" $list | tail -n 1
  n=$(wc -l < $list)
  [ "$n" = "$prev" ] && break
  prev=$n
done
