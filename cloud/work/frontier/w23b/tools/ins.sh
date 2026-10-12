#!/bin/sh
# w23b (builder, scratch root): instruction-text diff count (branch targets stripped) of the LOCK compile of SRC vs tgt/FN.o
# usage: sh lane/tools/ins.sh SRC FN [-d]
TK=$HOME/rush2049/cache/toolkits/4586d230749de1bc8a64add52ae53db0ffde18a1eee76c8a73f3791fd98e2557
src=$1; fn=$2; d=$(mktemp -d)
python3 -c "
import sys; sys.path[:0]=['.']
from tools.conveyor.seeds import context as c
open('$d/r.c','w').write(c.reduced_tu(open('$src').read(),'$fn'))"
fl=$(head -1 $src | sed -n 's@/\* flags: \(.*\) \*/@\1@p'); fl=${fl:--g0 -O2 -mips2 -G 0 -non_shared}
$TK/ido/cc -c $fl -Wab,-r4300_mul -I include -I include/PR -I $TK/shim -o $d/r.o $d/r.c 2>/dev/null || { echo COMPILE FAIL; exit 1; }
o() { mips-linux-gnu-objdump -d -j .text $1 | sed -n 's/^ *[0-9a-f]*:\s*[0-9a-f]\{8\}\s*//p' | sed 's/\s\+/ /g; s/ [0-9a-f]* <.*>$//'; }
o tgt/$fn.o > $d/t; o $d/r.o > $d/m
echo "ins=$(diff $d/t $d/m | grep -c '^[<>]') n=$(wc -l < $d/m)/$(wc -l < $d/t)"
[ "${3:-}" = -d ] && diff -y -W 100 $d/t $d/m | grep -n '[<>|]'
rm -rf $d
