#!/bin/sh
# w27s builder: dforce.sh SRC FN [CDX_FORCE spec] -> 5.3 cfe, TRACED uopt (wtk, CDX_LOG; optional CDX_FORCE on proc 0),
# stock ugen, then the ido5.3s assembly step (.loc/.option/comments dropped, cc -c -O1 -g); prints sum.sh of the
# colouring decisions (SUM=1) and the diff vs tgt/FN.o. Work dir $S/df_FN. Oracle only, never match evidence.
S=$HOME/rush2049/scratch/frontier/w27s; SRC=$1; FN=$2; F=$3
TKS=$HOME/rush2049/cache/toolkits/8bdf0caebc9ada9612b9dae9422948e37bd119e663e9dda8bb8fdbb7acd96682; I=$TKS/ido/ido53
X=$HOME/rush2049/scratch/frontier/wtk/bin
D=$S/df_$FN; rm -rf $D; mkdir -p $D; cd $D
python3 -c "
import sys; sys.path[:0]=['$S']
from tools.conveyor.seeds import context as c
open('r.c','w').write(c.reduced_tu(open('$SRC').read(),'$FN'))"
$I/cc -K -Hf -g0 -O2 -mips2 -G 0 -non_shared -I $S/include -I $S/include/PR -I $TKS/shim -c r.c >cfe.log 2>&1 || { cat cfe.log; exit 1; }
cp r.T st.run
if [ -n "$F" ]; then env CDX_PROC=${PROC:-0} CDX_FORCE="$F" CDX_LOG=1 CDX_DETAIL_WEB=all CDX_OUT=$D/d.txt $X/uopt -G 0 -mips2 -EB -g0 -O2 r.B r.O -t st.run optlog >uopt.log 2>&1
else env CDX_LOG=1 CDX_DETAIL_WEB=all CDX_OUT=$D/d.txt $X/uopt -G 0 -mips2 -EB -g0 -O2 r.B r.O -t st.run optlog >uopt.log 2>&1; fi
$I/ugen -G 0 -mips2 -EB -g0 -O2 r.O -o r.G -l r.s -t st.run -temp ugtmp >/dev/null 2>&1 || { echo ugen failed; exit 1; }
grep -v -E '^(	\.loc	|	\.option	O[0-9]|\s#)' r.s > rs.s
$I/cc -c -O1 -g -mips2 -G 0 -non_shared -o r.o rs.s >as.log 2>&1 || { cat as.log; exit 1; }
[ -n "$SUM" ] && sh $S/cloud/work/frontier/tools/trace/sum.sh d.txt
grep -E "force_declined|forced=[0-9]" d.txt | head -5
rows() { mips-linux-gnu-objdump -dr -j .text "$1" | sed -n '/^ *[0-9a-f]*:/p' | sed 's/^ *[0-9a-f]*:\s*//' | sed 's/\s\+/ /g'; }
rows $S/tgt/$FN.o > t.rows; rows r.o > r.rows
diff -y -W 130 t.rows r.rows | grep -n -E '\s[|<>]\s|\s<$|\s>$' > dd.txt; head -${N:-20} dd.txt
echo "rows: want $(wc -l < t.rows) got $(wc -l < r.rows); differing $(wc -l < dd.txt)"
