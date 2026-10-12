#!/bin/sh
# ON THE BUILDER: pre71r.sh SCR SRC FN [ENV=V ...]  -- compile FN's reduced TU with the PRE-oracle uopt (SCR/t71/pre),
# P71_FN=FN P71_OUT=SCR/t71/run/FN/p71.txt plus the given env (P71_CLR=..., P71_SET=..., CDX_*), then row diff.
S=$1; SRC=$2; FN=$3; shift 3
I=$S/t71/pre
TK=$HOME/rush2049/cache/toolkits/4586d230749de1bc8a64add52ae53db0ffde18a1eee76c8a73f3791fd98e2557
cd $S || exit 1
D=$S/t71/run/$FN; mkdir -p $D
fl=$(head -1 "$SRC" | sed -n 's@/\* flags: \(.*\) \*/@\1@p'); fl=${fl:--g0 -O2 -mips2 -G 0 -non_shared}
case " $fl " in *r4300_mul*) ;; *) fl="$fl -Wab,-r4300_mul" ;; esac
INC="-I $S/include -I $S/include/PR -I $TK/shim"
python3 -c "
import sys; sys.path[:0]=['$S']
from tools.conveyor.seeds import context as c
open('$D/r.c','w').write(c.reduced_tu(open('$SRC').read(),'$FN'))" || { echo "reduced_tu failed"; exit 1; }
rows() { mips-linux-gnu-objdump -dr -j .text "$1" | sed -n '/^ *[0-9a-f]*:/p' | sed 's/^ *[0-9a-f]*:\s*//' | sed 's/\s\+/ /g'; }
(cd $D && env P71_FN=$FN P71_OUT=$D/p71.txt "$@" $I/cc -c $fl $INC -o $D/p.o r.c) >$D/pcc.log 2>&1 || { cat $D/pcc.log; exit 1; }
rows $S/tgt/$FN.o > $D/t.rows; rows $D/p.o > $D/m.rows
[ -n "$DIFF" ] && diff -y -W 140 $D/t.rows $D/m.rows | grep -n -E '\s[|]\s|\s<$|\s>\s'
grep '\[P71\] oracle' $D/p71.txt
echo "rows: want $(wc -l < $D/t.rows) got $(wc -l < $D/m.rows); differing rows $(diff -y -W 140 $D/t.rows $D/m.rows | grep -c -E '\s[|]\s|\s<$|\s>\s') (p71 trace $D/p71.txt)"
