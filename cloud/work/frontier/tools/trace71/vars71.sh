#!/bin/sh
# usage: SCR=.. vars71.sh SRC FN  -- FN's declared locals and parameters with the virtual-frame offset the colouring
# trace prints as var@OFF (from a -g3 build of the same reduced TU; lib/mdvars.py). ctrace71.sh uses it to name webs.
. "$(dirname "$0")/lib/env71.sh"
src=$1; n=$2; b=$(push "$src") || exit 1
rtk vars $b $n
