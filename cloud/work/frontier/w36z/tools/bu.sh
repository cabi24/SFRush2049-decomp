#!/bin/sh
# bu.sh FILE.c: whole-program unit score of the results-table group file (all 8 functions) with E58 uargs.
cd /home/cburnes/projects/rush2049-decomp
python3 -m tools.conveyor.pipeline.blob_unit --tag w36z score func_80100E58 func_80102F30 drawMain func_80101904 func_80101D84 func_80102448 func_80100D5C drawBattleScore --with "$1" $(cat cloud/work/frontier/w33a/f0e58/uargs.txt) --internal rs_DrawEllipseText --internal rs_SetJust --neighbours 2>&1 | grep -E "FAIL|E58|differ in|blob_unit score|rror" | sed "s#^#$(basename $1): #"
