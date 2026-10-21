#!/bin/bash
V=$1
./mk.sh src/$V.c gv/$V >/dev/null && ../tools/gsc.sh a gv/$V | awk '/^func_803AA3D8:/{p=1} /^func_803AD57C:/{p=0} p' | grep -v "not verified"
../tools/gsc.sh a gv/$V | grep -B1 -E "^  [0-9]+/[0-9]+ words" | grep -v "^--" | grep -A1 -E "^func_803A[A-D]" | grep -v func_803AA3D8 | grep -E "words differ" | head -3
