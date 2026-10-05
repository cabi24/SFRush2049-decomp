#!/bin/sh
# runs ON THE BUILDER: asx_remote.sh file.s -> assemble a ugen listing with as0+as1 (-O3) and print objdump
TK=$HOME/rush2049/cache/toolkits/796ae99a5cb7922e335e3afd87008753c573c82b45d04e8cfbce4f513cdbfbf5
I=$TK/ido
d=$(mktemp -d); cp "$1" $d/x.s; cd $d
$I/as0 -G 0 -mips2 -EB -g0 -O3 x.s -o x.G -t st || exit 1
$I/as1 -elf -G 0 -p0 -mips2 -EB -g0 -O3 -r4300_mul -Olimit 5000 x.G -o x.o -t st || exit 1
mips-linux-gnu-objdump -d -z x.o | sed 's/^ *//' | cut -f1,3-
cd /; rm -rf $d
