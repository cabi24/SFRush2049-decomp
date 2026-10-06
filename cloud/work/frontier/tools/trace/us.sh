#!/bin/sh
# usage: TAG=.. us.sh NAME FILE.c [FILE.c...]   (UARGS="--internal X --keep Y" for extra blob_unit score args)
# Unit-score NAME with each file (whole-program shadow build, tag $TAG, 2 jobs) and print one line per file:
#   file: <verdict> | words N ops N norm N | frame want/got
# With one file and DIFF=1 the aligned diff (udiff.py $UD) follows. Replaces us.sh/ub.sh/fr.sh/q.sh/u.sh/gu.sh/r.sh/run.sh.
. "$(dirname "$0")/lib/env.sh"
n=$1; shift
for f in "$@"; do
  r=$(unit_score "$n" "$(realp "$f")" $UARGS | grep -E "(EQUAL|FAIL) $n|rror" | head -1 | sed 's/^ *//' | cut -c1-110)
  w=$(udiff $n --summary); o=$(udiff $n --summary --ops); z=$(udiff $n --summary --norm)
  rows() { echo "$1" | sed 's/.*differing rows \([0-9]*\).*/\1/'; }
  echo "$(basename $f): $r | words $(rows "$w") ops $(rows "$o") norm $(rows "$z") | frame $(echo "$w" | sed 's/.*frame \([0-9/]*\).*/\1/')"
done
if [ $# = 1 ] && [ -n "$DIFF" ]; then udiff $n $UD; fi
