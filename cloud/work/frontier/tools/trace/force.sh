#!/bin/sh
# usage: TAG=.. force.sh LABEL NAME SPEC [udiff args]  -- colouring oracle. Re-runs the traced uopt on snapshot st_LABEL
# with CDX_PROC=<NAME's ordinal> CDX_FORCE=SPEC, then stock ugen + as1, and prints:
#   the forced records (p?color ... forced=c / p?dec ... forced=-1 for =s) and every force_declined record,
#   then the aligned diff of NAME against retail (udiff args, e.g. --summary / --ops).
# SPEC: comma-separated, no spaces: p1:w70=c6 (phase 1 web 70 -> colour 6), p2:w9=c14, p1:w80=s (force split).
. "$(dirname "$0")/lib/env.sh"
lab=$1; n=$2; spec=$3; shift 3
rtk $lab force $n "'$spec'" || exit 1
fetch $lab f.o "$LOUT/f_$lab.o" && udiff $n --obj "$LOUT/f_$lab.o" "$@"
