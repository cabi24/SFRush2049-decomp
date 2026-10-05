#!/bin/sh
# usage: us.sh NAME file.c [blob_unit args] -- unit score + diff rows count
cd /home/cburnes/projects/rush2049-decomp
n=$1; f=$2; shift; shift
python3 -m tools.conveyor.pipeline.blob_unit --tag w2a score $n --with $f "$@" 2>&1 | grep -E "EQUAL|FAIL" | head -3
cloud/work/frontier/w2a/udiff.sh $n > /tmp/claude-1000/_w2a_d.txt
echo "diff rows: $(grep -c '[|<>]' /tmp/claude-1000/_w2a_d.txt)"
