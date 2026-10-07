#!/bin/sh
# vr.sh "NAMES" [extra args] -- FILES... : unit-score NAMES with each file, print verdict lines
OLDPWD=$(pwd); cd /home/cburnes/projects/rush2049-decomp
names=$1; shift
extra=""
while [ "$1" != "--" ]; do extra="$extra $1"; shift; done; shift
for f0 in "$@"; do f=$(realpath -m "$OLDPWD/$f0" 2>/dev/null); [ -f "$f" ] || f=$f0
  echo "== $f"
  python3 -m tools.conveyor.pipeline.blob_unit --tag w13d --jobs 2 score $names --with $f $extra --neighbours 2>&1 | grep -E "EQUAL|FAIL|locked bodies|rror"
done
