#!/bin/sh
# area.sh NAME CAND.c LABEL -- unit-score (tag w8c, $EXTRA), snapshot, run w6a's area-instrumented uopt; print AREA/GETTEMP lines for NAME
R=/home/cburnes/projects/rush2049-decomp; D=$(pwd); cd $R
name=$1; cand=$(cd "$D"; realpath "$2"); lab=$3
python3 -m tools.conveyor.pipeline.blob_unit --tag w8c score $name $EXTRA --with "$cand" 2>&1 | grep -E "EQUAL|FAIL|rror" | head -1
ssh watchman2 "cd ~/rush2049/scratch/frontier/w8c && rm -rf ar_$lab && mkdir ar_$lab && cp ../unit/w8c/stage/merged ../unit/w8c/stage/st ar_$lab/ && cd ar_$lab && cp st st.run && ../../w6a/uopt2/uopt -G 0 -Olimit 5000 -mips2 -EB -g0 -O3 merged opt -t st.run 2>err.log >/dev/null; grep -E 'proc=$name( |\$)' err.log"
