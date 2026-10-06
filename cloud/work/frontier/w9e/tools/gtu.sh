#!/bin/sh
# usage: gtu.sh LABEL PROC -- after a blob_unit score (tag w9e), snapshot the unit's merged ucode and run the w5d-traced
# uopt (level-3 PRE listing + colouring trace) for PROC; report in cloud/work/frontier/w9e/runs/LABEL/report.txt
R=/home/cburnes/projects/rush2049-decomp
lab=$1; proc=$2
OUT=$R/cloud/work/frontier/w9e/runs/$lab; mkdir -p $OUT
ssh watchman2 "W=\$HOME/rush2049/scratch/frontier; d=\$W/w9e/st_$lab; rm -rf \$d; mkdir -p \$d; cp \$W/unit/w9e/stage/merged \$W/unit/w9e/stage/st \$d/ && cd \$d && cp st st.run && W5D_LEVEL=3 W5D_PROC=$proc W5D_OUT=\$d/w5d CDX_LOG=1 CDX_DETAIL_WEB=all CDX_OUT=\$d/cdx \$W/w5d/uopt/uopt -G 0 -Olimit 5000 -mips2 -EB -g0 -O3 merged opt -t st.run -l list >uopt.log 2>&1; grep -v procindex cdx > cdx.p; mv cdx.p cdx; awk '/LOCAL OPTIMIZATION OF $proc\$/{p=1} p{print} /REEMISSION OF $proc\$/{exit}' list > list.p; mv list.p list; ls -la"
scp -q watchman2:rush2049/scratch/frontier/w9e/st_$lab/list watchman2:rush2049/scratch/frontier/w9e/st_$lab/w5d watchman2:rush2049/scratch/frontier/w9e/st_$lab/cdx $OUT/
python3 $R/cloud/work/frontier/w5d/tools/prereport.py $OUT $proc > $OUT/report.txt
echo "report: $OUT/report.txt"
