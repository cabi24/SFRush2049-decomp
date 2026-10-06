#!/bin/sh
# usage: TAG=.. snap.sh LABEL                    snapshot the last unit build of $TAG (merged + st) into $SCR/st_LABEL
#        TAG=.. snap.sh LABEL FILE.c KEEP,LIST    or stage a single-file -O3 group (cc -j, uld -kp KEEP, usplit, umerge)
# Every tracer below works on a snapshot, so a later `us.sh` (which rebuilds the unit) does not disturb it.
. "$(dirname "$0")/lib/env.sh"
if [ -n "$2" ]; then
  rsh "mkdir -p ~/$SCR/cand" && scp -q "$(realp "$2")" "$BUILDER:$SCR/cand/_g_$1.c" && rtk $1 gstage "~/$SCR/cand/_g_$1.c" $3
else snap_unit $1 && echo "snapshot $BUILDER:~/$SCR/st_$1 (unit $TAG)"; fi
