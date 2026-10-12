#!/bin/sh
# w23b (builder): reassemble listing FILE.s and diff vs the direct compile of SRC (fidelity check), FN
TK=$HOME/rush2049/cache/toolkits/4586d230749de1bc8a64add52ae53db0ffde18a1eee76c8a73f3791fd98e2557
s=$1; src=$2; fn=$3; d=$(mktemp -d -p lane)
cp $s $d/r.s; (cd $d && $TK/ido/cc -c -g0 -O2 -mips2 -G 0 -non_shared r.s -o r.o) || exit 1
sh lane/tools/obj.sh $src $fn $d/c.o
o() { mips-linux-gnu-objdump -d -j .text $1 | sed -n '/^ *[0-9a-f]*:/p' | sed 's/^ *[0-9a-f]*:\s*//' | sed 's/\s\+/ /g'; }
o $d/c.o > $d/t; o $d/r.o > $d/m; diff -y -W 140 $d/t $d/m | grep '|\|<\|>' | head -${SHOW:-20}; rm -rf $d
