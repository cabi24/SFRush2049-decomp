#!/bin/sh
# usage: TAG=.. [VEC=d|s|u] [FORCE=SPEC] nopre.sh LABEL NAME BITS [udiff args]  -- PRE oracle: clear expression BITS
# (comma list, bit numbers from pretrace.sh) from NAME's delete vector after code motion ("what if PRE had not removed
# these occurrences?"), optionally CDX_FORCE=SPEC on top, then ugen + as1 and the aligned diff of NAME.
# Insertions (i/h) cannot be cleared (codemotion has already materialised them; uopt dies).
. "$(dirname "$0")/lib/env.sh"
lab=$1; n=$2; bits=$3; shift 3
rtk $lab nopre $n $bits ${FORCE:+"'$FORCE'"} || exit 1
fetch $lab n.o "$LOUT/n_$lab.o" && udiff $n --obj "$LOUT/n_$lab.o" "$@"
