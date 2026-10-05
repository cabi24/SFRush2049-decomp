#!/bin/sh
# usage: [EXTRA="--internal X --keep Y"] [TAG=w5d] pretrace.sh NAME CAND.c LABEL [PROC]
# Scores NAME with CAND.c in the whole-program unit (blob_unit --tag $TAG), snapshots the stage's merged
# ucode + symbol table on the builder as ~/rush2049/scratch/frontier/w5d/st_LABEL, runs the w5d uopt on it
# with the listing (-l), uopt's level-3 dump, the w5d bit/regcand trace and the workbench colouring trace,
# all restricted to procedure PROC (default NAME), and copies list/w5d/cdx to
# $OUT (default cloud/work/frontier/w5d/runs/LABEL), then prints prereport.py's summary.
D=$(pwd); R=/home/cburnes/projects/rush2049-decomp; cd $R
name=$1; cand=$(cd "$D"; realpath "$2"); lab=$3; proc=${4:-$1}; T=${TAG:-w5d}
OUT=${OUT:-$R/cloud/work/frontier/w5d/runs/$lab}
python3 -m tools.conveyor.pipeline.blob_unit --tag $T score $name $EXTRA --with "$cand" 2>&1 | grep -E "EQUAL|FAIL|rror" | head -3
ssh watchman2 "cd ~/rush2049/scratch/frontier/w5d && rm -rf st_$lab && mkdir st_$lab && cp ../unit/$T/stage/merged ../unit/$T/stage/st st_$lab/ && cd st_$lab && cp st st.run && W5D_LEVEL=3 W5D_PROC=$proc W5D_OUT=\$PWD/w5d CDX_LOG=1 CDX_DETAIL_WEB=all CDX_OUT=\$PWD/cdx ../uopt/uopt -G 0 -Olimit 5000 -mips2 -EB -g0 -O3 merged opt -t st.run -l list >uopt.log 2>&1; grep -v procindex cdx > cdx.p; mv cdx.p cdx; awk '/LOCAL OPTIMIZATION OF $proc\$/{p=1} p{print} /REEMISSION OF $proc\$/{exit}' list > list.p; mv list.p list"
mkdir -p "$OUT"
scp -q watchman2:rush2049/scratch/frontier/w5d/st_$lab/list watchman2:rush2049/scratch/frontier/w5d/st_$lab/w5d watchman2:rush2049/scratch/frontier/w5d/st_$lab/cdx "$OUT/"
python3 $R/cloud/work/frontier/w5d/tools/prereport.py "$OUT" $proc > "$OUT/report.txt"
cp "$cand" "$OUT/source.c"
echo "report: $OUT/report.txt"
