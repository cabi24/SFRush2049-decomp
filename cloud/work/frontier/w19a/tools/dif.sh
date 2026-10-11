#!/bin/sh
# w18c: side-by-side objdump -dr of the LOCK compile of SRC FN vs tgt/FN.o (IDO 7.1)
TK=$HOME/rush2049/cache/toolkits/4586d230749de1bc8a64add52ae53db0ffde18a1eee76c8a73f3791fd98e2557
src=$1; fn=$2; d=$(mktemp -d)
python3 -c "
import sys; sys.path[:0]=['.']
from tools.conveyor.seeds import context as c
open('$d/r.c','w').write(c.reduced_tu(open('$src').read(),'$fn'))"
fl=$(head -1 $src | sed -n 's@/\* flags: \(.*\) \*/@\1@p'); fl=${fl:--g0 -O2 -mips2 -G 0 -non_shared}
$TK/ido/cc -c $fl -I include -I include/PR -I $TK/shim -o $d/r.o $d/r.c || exit 1
o() { mips-linux-gnu-objdump -dr -j .text $1 | sed -n '/^ *[0-9a-f]*:/p' | sed 's/^ *[0-9a-f]*:\s*//' | sed 's/\s\+/ /g'; }
o tgt/$fn.o > $d/t; o $d/r.o > $d/m
diff -y -W 140 $d/t $d/m | ${PAGER_CMD:-cat}; rm -rf $d
