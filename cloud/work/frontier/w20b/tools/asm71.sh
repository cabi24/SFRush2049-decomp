#!/bin/sh
# w20b (builder, scratch root): assemble an (edited) ugen listing with IDO 7.1 cc (as1 schedules) and diff vs tgt/FN.o
TK=$HOME/rush2049/cache/toolkits/4586d230749de1bc8a64add52ae53db0ffde18a1eee76c8a73f3791fd98e2557
s=$1; fn=$2; d=$(mktemp -d -p lane); cp $s $d/x.s
(cd $d && $TK/ido/cc -c -g0 -O2 -mips2 -G 0 -non_shared x.s) || exit 1
o() { mips-linux-gnu-objdump -dr -j .text $1 | sed -n '/^ *[0-9a-f]*:/p' | sed 's/^ *[0-9a-f]*:\s*//' | sed 's/\s\+/ /g'; }
o tgt/$fn.o > $d/t; o $d/x.o > $d/m; diff -y -W 140 $d/t $d/m | grep -n -E '\s[|]\s|\s<$|\s>\s'; echo "diffrows=$(diff $d/t $d/m | grep -c '^[<>]')"; rm -rf $d
