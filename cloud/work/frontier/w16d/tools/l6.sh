#!/bin/sh
# l6.sh LABEL CAND.c [NAME]: unit-score NAME (default camera_victory) with CAND.c (tag w16d), snapshot it as st_LABEL,
# rerun the traced uopt at debug level 6 (W5D_LEVEL=6: colouring/split detail) for NAME and fetch the listing's
# colouring section to build/trace/w16d/LABEL.l6. Also runs ctrace.sh (CDX records) on the same snapshot.
set -e
L=$1; F=$(realpath "$2"); N=${3:-camera_victory}
export TAG=w16d
T=/home/cburnes/projects/rush2049-decomp/cloud/work/frontier/tools/trace
O=/home/cburnes/projects/rush2049-decomp/build/trace/w16d
$T/ctrace.sh $N $L $F > $O/$L.ctrace
ssh watchman2 "cd ~/rush2049/scratch/frontier/w16d/st_$L && cp st st.l6 && W5D_LEVEL=6 W5D_PROC=$N ../bin/uopt -G 0 -Olimit 5000 -mips2 -EB -g0 -O3 merged opt.l6 -t st.l6 -l list6 >/dev/null 2>&1; awk '/reg alloc preparation of $N\$/{p=1} p{print} /global coloring of $N\$/{exit}' list6" > $O/$L.l6
head -1 $O/$L.ctrace; wc -l $O/$L.l6
