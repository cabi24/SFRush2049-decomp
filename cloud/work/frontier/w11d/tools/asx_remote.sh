#!/bin/sh
# ON BUILDER: asx_remote.sh FN.s FN -- as0+as1 a (possibly hand-edited) single-function ugen listing; print disassembly
I=$HOME/rush2049/cache/toolkits/796ae99a5cb7922e335e3afd87008753c573c82b45d04e8cfbce4f513cdbfbf5/ido
d=$(mktemp -d)
{ printf '\t.verstamp\t3 19\n\t.option\tpic0\n\t.text\n\t.align\t2\n'; cat "$1"; } > $d/f.s
$I/as0 -G 0 -mips2 -EB -g0 -O3 $d/f.s -o $d/m.b -t $d/m.st > $d/as0.log 2>&1 || { echo as0fail; head $d/as0.log; exit 1; }
$I/as1 -elf -G 0 -p0 -mips2 -EB -g0 -O3 -r4300_mul -Olimit 5000 $d/m.b -o $d/m.o -t $d/m.st >/dev/null 2>&1 || { echo as1fail; exit 1; }
mips-linux-gnu-objdump -d -z $d/m.o | grep -E '^ +[0-9a-f]+:' | sed 's/^ *\([0-9a-f]*\):\t[0-9a-f ]*\t/\1 /; s/\t/ /g' 
rm -rf $d
