#!/bin/sh
# builder: urun.sh DIR FILE.O FN -> ugen + as1 (traced 7.1 passes) on an (edited) uopt output, row diff vs tgt
S=$HOME/rush2049/scratch/frontier/w25f; I=$S/t71/ido; D=$1; O=$2; FN=$3
cd $D || exit 1
b=$(basename $O .O)
DKWB_UGEN_TRACE=1 $I/ugen -G 0 -mips2 -EB -g0 -O2 $O -o $b.G -l $b.lst -t $D/r.T -temp $D/$b.tmp 2>$b.ugt >/dev/null || { tail -3 $b.ugt; exit 1; }
$I/as1 -t5_ll_sc_bug -elf -G 0 -p0 -r4300_mul -r4300_mul -mips2 -EB -g0 -O2 $b.G -o $b.o -t $D/r.T >/dev/null 2>&1 || { echo as1 failed; exit 1; }
rows() { mips-linux-gnu-objdump -dr -j .text "$1" | sed -n '/^ *[0-9a-f]*:/p' | sed 's/^ *[0-9a-f]*:\s*//' | sed 's/\s\+/ /g'; }
rows $S/tgt/$FN.o > t.rows; rows $b.o > $b.rows
echo "rows: want $(wc -l < t.rows) got $(wc -l < $b.rows); differing rows $(diff -y -W 140 t.rows $b.rows | grep -c -E '\s[|]\s|\s<$|\s>\s')"
