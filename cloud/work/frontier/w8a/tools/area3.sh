#!/bin/sh
# area3.sh NAME CAND.c LABEL -- unit-score (tag w8a, $EXTRA), snapshot, run uopt3 (area+spill+home trace); print lines for NAME
R=/home/cburnes/projects/rush2049-decomp; D=$(pwd); cd $R
name=$1; cand=$(cd "$D"; realpath "$2"); lab=$3
python3 -m tools.conveyor.pipeline.blob_unit --tag w8a score $name $EXTRA --with "$cand" 2>&1 | grep -E "EQUAL|FAIL|rror" | head -1
ssh watchman2 "cd ~/rush2049/scratch/frontier/w8a && rm -rf ar_$lab && mkdir ar_$lab && cp ../unit/w8a/stage/merged ../unit/w8a/stage/st ar_$lab/ && cd ar_$lab && cp st st.run && ../uopt3/uopt -G 0 -Olimit 5000 -mips2 -EB -g0 -O3 merged opt -t st.run 2>err.log >/dev/null; grep -E 'proc=$name( |\$)' err.log"
