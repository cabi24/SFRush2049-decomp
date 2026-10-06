#!/bin/sh
# ON BUILDER (cand/): asd_remote.sh file.s FUNC -> as0+as1 (-O3 pipeline flags) a ugen listing, objdump FUNC
TK=$HOME/rush2049/cache/toolkits/796ae99a5cb7922e335e3afd87008753c573c82b45d04e8cfbce4f513cdbfbf5
I=$TK/ido
d=$(mktemp -d)
$I/as0 -G 0 -mips2 -EB -g0 -O3 "$1" -o $d/m.b -t $d/m.st || exit 1
$I/as1 -elf -G 0 -p0 -mips2 -EB -g0 -O3 -r4300_mul -Olimit 5000 $d/m.b -o $d/m.o -t $d/m.st || exit 1
OD=$(command -v mips-linux-gnu-objdump || command -v mips64-linux-gnu-objdump)
$OD -d --disassemble=$2 $d/m.o | tail -n +7
rm -rf $d
