#!/bin/sh
# usage: TAG=.. [SCR=..] fidelity.sh [NAME CAND.c]  -- prove the traced binaries in $SCR/bin are byte-identical to
# stock with tracing off AND on, on the whole-program unit. With NAME CAND.c the unit is scored first; without, the
# last unit build of $TAG is used. Snapshot: $SCR/st_fid.
. "$(dirname "$0")/lib/env.sh"
[ -n "$2" ] && unit_score "$1" "$(realp "$2")"
snap_unit fid
scp -q "$TK"/remote/*.sh "$BUILDER:$SCR/tk/"
rsh "sh ~/$SCR/tk/fidelity.sh ~/$SCR fid ~/rush2049/scratch/frontier/unit/$TAG/stage/unit.o"
