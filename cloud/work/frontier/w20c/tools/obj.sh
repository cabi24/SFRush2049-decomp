#!/bin/sh
# w20c (builder, scratch root): compile SRC's FN as `lock add` does (reduced_tu, LOCK flags, IDO 7.1 toolkit 4586d230) -> OUT.o
TK=$HOME/rush2049/cache/toolkits/4586d230749de1bc8a64add52ae53db0ffde18a1eee76c8a73f3791fd98e2557
src=$1; fn=$2; out=$3; d=$(mktemp -d -p .)
python3 -c "
import sys; sys.path[:0]=['.']
from tools.conveyor.seeds import context as c
open('$d/r.c','w').write(c.reduced_tu(open('$src').read(),'$fn'))"
$TK/ido/cc -c -g0 -O2 -mips2 -G 0 -non_shared -I include -I include/PR -I $TK/shim -o $out $d/r.c; rc=$?; rm -rf $d; exit $rc
