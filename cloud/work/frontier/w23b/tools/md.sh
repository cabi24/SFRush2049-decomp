#!/bin/sh
# w23b (builder, scratch root): mdebug local offsets of SRC's FN (reduced TU, IDO 7.1 -g3 -O2): sh lane/tools/md.sh SRC FN
TK=$HOME/rush2049/cache/toolkits/4586d230749de1bc8a64add52ae53db0ffde18a1eee76c8a73f3791fd98e2557
d=$(mktemp -d -p lane); python3 -c "
import sys; sys.path[:0]=['.']
from tools.conveyor.seeds import context as c
open('$d/r.c','w').write(c.reduced_tu(open('$1').read(),'$2'))"
$TK/ido/cc -c -g3 -O2 -mips2 -G 0 -non_shared -I include -I include/PR -I $TK/shim -o $d/r.o $d/r.c && python3 lane/tools/mdlocals.py $d/r.o | grep -E '^(Local|Param)'; rm -rf $d
