#!/bin/sh
# w26b builder: kc.sh SRC FN DIR -> reduced TU DIR/r.c, cc -K (t71/ido) keeps DIR/r.O r.T r.B; then urun on r.O
S=$HOME/rush2049/scratch/frontier/w27s; SRC=$1; FN=$2; D=$3
TK=$HOME/rush2049/cache/toolkits/4586d230749de1bc8a64add52ae53db0ffde18a1eee76c8a73f3791fd98e2557
mkdir -p $D
fl=$(head -1 "$SRC" | sed -n 's@/\* flags: \(.*\) \*/@\1@p'); fl=${fl:--g0 -O2 -mips2 -G 0 -non_shared}
case " $fl " in *r4300_mul*) ;; *) fl="$fl -Wab,-r4300_mul" ;; esac
INC="-I $S/include -I $S/include/PR -I $TK/shim"
python3 -c "
import sys; sys.path[:0]=['$S']
from tools.conveyor.seeds import context as c
open('$D/r.c','w').write(c.reduced_tu(open('$SRC').read(),'$FN'))" || { echo "reduced_tu failed"; exit 1; }
(cd $D && $S/t71/ido/cc -K -c $fl $INC -o $D/k.o r.c) >$D/kcc.log 2>&1 || { cat $D/kcc.log; exit 1; }
sh $S/lane/tools/urun.sh $D $D/r.O $FN
