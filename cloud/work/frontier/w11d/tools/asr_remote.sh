#!/bin/sh
# ON BUILDER (w11d scratch): asr_remote.sh FN.s OUTLOG -- as0 + traced as1 -R on a single-function listing
I=$HOME/rush2049/cache/toolkits/796ae99a5cb7922e335e3afd87008753c573c82b45d04e8cfbce4f513cdbfbf5/ido
A=$HOME/rush2049/scratch/frontier/w11d/as1/as1
d=$(mktemp -d)
{ printf '\t.verstamp\t3 19\n\t.option\tpic0\n\t.text\n\t.align\t2\n'; cat "$1"; } > $d/f.s
$I/as0 -G 0 -mips2 -EB -g0 -O3 $d/f.s -o $d/m.b -t $d/m.st >/dev/null 2>&1 || exit 1
cp $d/m.st $d/m2.st
$I/as1 -elf -G 0 -p0 -mips2 -EB -g0 -O3 -r4300_mul -Olimit 5000 $d/m.b -o $d/m.o -t $d/m.st || exit 1
$A -elf -G 0 -p0 -mips2 -EB -g0 -O3 -r4300_mul -Olimit 5000 -R $d/m.b -o $d/r.o -t $d/m2.st > "$2" 2>&1
cmp $d/m.o $d/r.o && echo identical
rm -rf $d
