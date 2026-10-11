#!/bin/sh
# w21a (builder): compile SRC's FN (reduced TU, LOCK flags, IDO 7.1) to OUT.o
TK=$HOME/rush2049/cache/toolkits/4586d230749de1bc8a64add52ae53db0ffde18a1eee76c8a73f3791fd98e2557
src=$1; fn=$2; out=$3; d=$(mktemp -d -p lane)
python3 -c "
import sys; sys.path[:0]=['.']
from tools.conveyor.seeds import context as c
open('$d/r.c','w').write(c.reduced_tu(open('$src').read(),'$fn'))"
fl=$(head -1 $src | sed -n 's@/\* flags: \(.*\) \*/@\1@p'); fl=${fl:--g0 -O2 -mips2 -G 0 -non_shared}
$TK/ido/cc -c $fl -I include -I include/PR -I $TK/shim -o $out $d/r.c; rm -rf $d
