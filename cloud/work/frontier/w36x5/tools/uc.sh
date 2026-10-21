#!/bin/sh
# usage (builder): uc.sh FILE.c [PROC] -- colouring trace of PROC (default entity_ai_pathfind) in a single-file group
S=$HOME/rush2049/scratch/frontier/w36x5
sh $S/uv.sh "$1" entity_ai_pathfind >/dev/null
cd $S/uvwork && cp st st.run && env W5D_PROC=${2:-entity_ai_pathfind} CDX_LOG=1 CDX_DETAIL_WEB=all CDX_OUT=$PWD/d.txt ../bin/uopt -G 0 -Olimit 5000 -mips2 -EB -g0 -O3 merged opt.x -t st.run optlog >/dev/null 2>&1
P=$(grep -m1 -o "proc=[0-9]*" d.txt | head -1)
grep -E "\[CDX\]" d.txt > cdx.all
echo "$(wc -l < cdx.all) records"
