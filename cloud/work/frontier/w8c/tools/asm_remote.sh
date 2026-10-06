#!/bin/sh
# ON BUILDER (cand/): asm_remote.sh file.s NAME -> assemble a (hand-edited) ugen listing with as0+as1 exactly like the -O3 pipeline and print aligned diff vs retail
TK=$HOME/rush2049/cache/toolkits/796ae99a5cb7922e335e3afd87008753c573c82b45d04e8cfbce4f513cdbfbf5
I=$TK/ido
d=$(mktemp -d)
$I/as0 -G 0 -mips2 -EB -g0 -O3 "$1" -o $d/m.b -t $d/m.st || exit 1
$I/as1 -elf -G 0 -p0 -mips2 -EB -g0 -O3 -r4300_mul -Olimit 5000 $d/m.b -o $d/m.o -t $d/m.st || exit 1
cd ~/rush2049/scratch/frontier/w8c
IDO_DIR=$I python3 cand/objdiff.py $d/m.o "$2" $3
rm -rf $d
