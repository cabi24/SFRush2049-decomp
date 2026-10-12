#!/bin/sh
# usage: SCR=.. fidelity71.sh [TU ...]  -- traced 7.1 == stock 7.1, on the boot-tail TUs in the builder scratch
# (default all 18 IDO 7.1 segments; SCR needs src/rom + include synced from the current tree). See remote/fidelity71.sh.
. "$(dirname "$0")/lib/env71.sh"
scp -q "$TK"/remote/fidelity71.sh "$BUILDER:$SCR/t71/fidelity71.sh" && rsh "nice -n 10 sh ~/$SCR/t71/fidelity71.sh ~/$SCR $*" | tee "$LOUT/fidelity71.log"
