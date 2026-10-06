#!/bin/sh
# ON BUILDER (cand/): o3d_remote.sh file.c keep1,keep2 FUNC -> -O3 whole-group pipeline through as1, objdump FUNC
TK=$HOME/rush2049/cache/toolkits/796ae99a5cb7922e335e3afd87008753c573c82b45d04e8cfbce4f513cdbfbf5
I=$TK/ido
d=$(mktemp -d); cp "$1" $d/g.c; cd $d
for k in $(echo $2 | tr , ' '); do echo $k >> keep.txt; done
$I/cc -j -g0 -O3 -mips2 -G 0 -non_shared g.c || exit 1
$I/uld -L/usr/lib/mips2/nonshared -_SYSTYPE_SVR4 -mips2 -non_shared -g0 -no_AutoGnum -kp keep.txt g.u -ko linked || exit 1
$I/usplit -mips2 -o split -t st linked || exit 1
$I/umerge -Olimit 5000 -mips2 -EB -g0 -O3 split -o merged -t st || exit 1
$I/uopt -G 0 -Olimit 5000 -mips2 -EB -g0 -O3 merged opt -t st optlog || exit 1
$I/ugen -G 0 -mips2 -EB -g0 -O3 opt -o gen -l out.s -t st -temp ugtmp || exit 1
$I/as1 -elf -G 0 -p0 -mips2 -EB -g0 -O3 -r4300_mul -Olimit 5000 gen -o m.o -t st || exit 1
OD=$(command -v mips-linux-gnu-objdump || command -v mips64-linux-gnu-objdump || ls $HOME/rush2049/cache/toolkits/*/bin/*objdump 2>/dev/null | head -1)
$OD -d -r --disassemble=$3 m.o | tail -n +7
cd /; rm -rf $d
