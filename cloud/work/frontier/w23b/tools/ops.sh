#!/bin/sh
# w23b (builder, scratch root): mnemonic-sequence diff of the LOCK compile of SRC vs tgt/FN.o.
# usage: sh lane/tools/ops.sh SRC FN [-d]   prints "ops=<changed lines> frame=<addiu sp>" (+ the diff with -d)
TK=$HOME/rush2049/cache/toolkits/4586d230749de1bc8a64add52ae53db0ffde18a1eee76c8a73f3791fd98e2557
src=$1; fn=$2; d=$(mktemp -d)
python3 -c "
import sys; sys.path[:0]=['.']
from tools.conveyor.seeds import context as c
open('$d/r.c','w').write(c.reduced_tu(open('$src').read(),'$fn'))"
fl=$(head -1 $src | sed -n 's@/\* flags: \(.*\) \*/@\1@p'); fl=${fl:--g0 -O2 -mips2 -G 0 -non_shared}
$TK/ido/cc -c $fl -I include -I include/PR -I $TK/shim -o $d/r.o $d/r.c 2>/dev/null || { echo COMPILE FAIL; exit 1; }
o() { mips-linux-gnu-objdump -d -j .text $1 | sed -n 's/^ *[0-9a-f]*:\s*[0-9a-f]\{8\}\s*\(\S*\).*/\1/p'; }
o tgt/$fn.o > $d/t; o $d/r.o > $d/m
echo "ops=$(diff $d/t $d/m | grep -c '^[<>]') frame=$(mips-linux-gnu-objdump -d -j .text $d/r.o | grep -m1 'addiu.*sp,sp,-' | awk '{print $NF}')"
[ "${3:-}" = -d ] && diff -y -W 60 $d/t $d/m | grep -n '[<>|]'
rm -rf $d
