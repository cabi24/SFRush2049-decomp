#!/bin/sh
# try.sh V.c [FN]: unit score (us.sh) + ops diff rows for one variant (serialised, TAG=w23e).
cd /home/cburnes/projects/rush2049-decomp
FN=${2:-entity_render_setup}
export TAG=w23e
cloud/work/frontier/tools/trace/us.sh $FN "$1" | tail -1
[ -n "$Q" ] || python3 cloud/work/frontier/tools/trace/udiff.py $FN --ops | grep -v '^want' | head -${ROWS:-40}
