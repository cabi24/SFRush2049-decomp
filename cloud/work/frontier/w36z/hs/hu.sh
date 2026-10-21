#!/bin/sh
# hu.sh FILE.c: unit-score the high-score file (renamed helpers) for all four functions.
cd /home/cburnes/projects/rush2049-decomp
D=cloud/work/frontier/w36z/hs
$D/ren.sh "$1" $D/_z_$(basename $1)
python3 -m tools.conveyor.pipeline.blob_unit --tag w36z score highscore_animText DrawName DrawGhost DrawUnlocked --with $D/_z_$(basename $1) $(cat $D/uargs.txt) --neighbours 2>&1 | grep -E "EQUAL|FAIL|differ in|rror" | sed "s#^#$(basename $1): #"
