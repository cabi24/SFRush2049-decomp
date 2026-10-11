#!/bin/sh
# w20b (builder, scratch root): IDO 7.1 `cc -S` (ugen listing, as1 input) of SRC's FN reduced TU -> OUT.s
TK=$HOME/rush2049/cache/toolkits/4586d230749de1bc8a64add52ae53db0ffde18a1eee76c8a73f3791fd98e2557
src=$1; fn=$2; out=$3; d=$(mktemp -d -p lane)
python3 -c "
import sys; sys.path[:0]=['.']
from tools.conveyor.seeds import context as c
open('$d/r.c','w').write(c.reduced_tu(open('$src').read(),'$fn'))"
fl=$(head -1 $src | sed -n 's@/\* flags: \(.*\) \*/@\1@p'); fl=${fl:--g0 -O2 -mips2 -G 0 -non_shared}
(cd $d && $TK/ido/cc -S $fl -I ../../include -I ../../include/PR -I $TK/shim r.c) && cp $d/r.s $out; rm -rf $d
