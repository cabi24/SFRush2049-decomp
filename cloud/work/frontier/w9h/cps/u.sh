#!/bin/sh
# usage: u.sh body.c [--all] -- unit-score camera_play_script with prefix+types+body; aligned diff
D=$(cd $(dirname $0) && pwd); b=$(realpath $1); shift
cat $D/prefix.c $D/types.c $b > $D/_cand.c
cd /home/cburnes/projects/rush2049-decomp
python3 -m tools.conveyor.pipeline.blob_unit --tag w9h --jobs 2 score camera_play_script --with $D/_cand.c 2>&1 | grep -E "EQUAL|FAIL|rror|differ:" | head -3
python3 cloud/work/frontier/w3a/tools/udiff.py camera_play_script --obj build/blob_unit/w9h/unit.o "$@"
