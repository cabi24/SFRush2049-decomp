#!/bin/sh
# usage: TAG=.. asm.sh LABEL FILE.s NAME [udiff args]  -- assemble a hand-edited ugen listing (e.g. from as1t.sh) with
# stock as0 + as1 exactly as the -O3 pipeline does, and diff NAME against retail. Answers "is this residual as1's
# (schedule/delay slot) or upstream's?": if editing the listing fixes it, look at as1 inputs (lines, memory facts).
. "$(dirname "$0")/lib/env.sh"
lab=$1; f=$(realp "$2"); n=$3; shift 3
scp -q "$f" "$BUILDER:$SCR/st_$lab/edit.s" && rtk $lab asm "~/$SCR/st_$lab/edit.s" >/dev/null && fetch $lab asm.o "$LOUT/asm_$lab.o" && udiff $n --obj "$LOUT/asm_$lab.o" "$@"
