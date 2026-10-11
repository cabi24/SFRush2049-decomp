#!/bin/sh
# w20a (builder, scratch root): assemble an IDO `-S` listing (full r.s) with IDO 7.1 cc (as0+as1) and diff vs tgt/FN.o
# usage: sh lane/tools/asm.sh FILE.s FN
TK=$HOME/rush2049/cache/toolkits/4586d230749de1bc8a64add52ae53db0ffde18a1eee76c8a73f3791fd98e2557
s=$1; fn=$2; d=$(mktemp -d -p lane)
cp $s $d/r.s; (cd $d && $TK/ido/cc -c -g0 -O2 -mips2 -G 0 -non_shared r.s -o r.o) || exit 1
o() { mips-linux-gnu-objdump -d -j .text $1 | sed -n '/^ *[0-9a-f]*:/p' | sed 's/^ *[0-9a-f]*:\s*//' | sed 's/\s\+/ /g'; }
o tgt/$fn.o > $d/t; o $d/r.o > $d/m
diff -y -W 140 $d/t $d/m | grep -c '|\|<\|>' ; [ -n "$SHOW" ] && diff -y -W 140 $d/t $d/m | head -${SHOW}; rm -rf $d
