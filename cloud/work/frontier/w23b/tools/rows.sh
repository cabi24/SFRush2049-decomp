#!/bin/sh
# w23b (builder, scratch root): objdump -dr rows of the LOCK compile of SRC FN vs tgt/FN.o; prints differing row pairs
# with a histogram by kind. usage: sh lane/tools/rows.sh SRC FN
TK=$HOME/rush2049/cache/toolkits/4586d230749de1bc8a64add52ae53db0ffde18a1eee76c8a73f3791fd98e2557
src=$1; fn=$2; d=$(mktemp -d)
python3 -c "
import sys; sys.path[:0]=['.']
from tools.conveyor.seeds import context as c
open('$d/r.c','w').write(c.reduced_tu(open('$src').read(),'$fn'))"
fl=$(head -1 $src | sed -n 's@/\* flags: \(.*\) \*/@\1@p'); fl=${fl:--g0 -O2 -mips2 -G 0 -non_shared}
$TK/ido/cc -c $fl -I include -I include/PR -I $TK/shim -o $d/r.o $d/r.c 2>/dev/null || { echo COMPILE FAIL; exit 1; }
o() { mips-linux-gnu-objdump -dr -j .text $1 | sed -n 's/^ *[0-9a-f]*:\s*\([0-9a-f]\{8\}\)\s.*/\1/p; s/^\s*\([0-9a-f]*\):\s*\(R_MIPS_\w*\)\s*\(\S*\)/\1 \2 \3/p'; }
o tgt/$fn.o > $d/t; o $d/r.o > $d/m
paste -d'|' $d/t $d/m | awk -F'|' '$1!=$2' > $d/diff
echo "differing rows: $(wc -l < $d/diff)"
grep -v R_MIPS $d/diff; awk -F'|' '{split($1,a," "); split($2,b," "); print a[2]" "a[3]" -> "b[3]}' $d/diff | sed 's/+0x[0-9a-f]*//g' | sort | uniq -c
rm -rf $d
