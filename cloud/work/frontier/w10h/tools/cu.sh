#!/bin/bash
# usage: cu.sh body.c [LABEL] -- unit-score camera_play_script (w10h tag) with w9h prefix+types; aligned diff summary; snapshot stage to st_LABEL
R=/home/cburnes/projects/rush2049-decomp; D=$R/cloud/work/frontier/w10h/cps; b=$(realpath $1); lab=${2:-cps}
cat $D/prefix.c $D/types.c $b > $D/_cand.c
cd $R
python3 -m tools.conveyor.pipeline.blob_unit --tag w10h --jobs 2 score camera_play_script --with $D/_cand.c 2>&1 | grep -E "EQUAL|FAIL|rror|locked bodies" | head -3 | cut -c1-150
python3 cloud/work/frontier/w3a/tools/udiff.py camera_play_script --obj build/blob_unit/w10h/unit.o | tail -${UT:-2}
ssh watchman2 "cd ~/rush2049/scratch/frontier/w10h && rm -rf st_$lab && mkdir st_$lab && cp ../unit/w10h/stage/merged ../unit/w10h/stage/st st_$lab/"
