#!/bin/sh
# ON BUILDER (cand/): pipe_remote.sh file.c keep NAME "<cc flags>" "<umerge flags>" "<uopt flags>" "<ugen flags>" -> counts .noalias in ugen listing and prints diff summary vs retail
TK=$HOME/rush2049/cache/toolkits/796ae99a5cb7922e335e3afd87008753c573c82b45d04e8cfbce4f513cdbfbf5
I=$TK/ido
d=$(mktemp -d); cp "$1" $d/g.c; here=$(pwd); cd $d
for k in $(echo $2 | tr , ' '); do echo $k >> keep.txt; done
$I/cc -j -g0 -O3 -mips2 -G 0 -non_shared $4 g.c || exit 1
$I/uld -L/usr/lib/mips2/nonshared -_SYSTYPE_SVR4 -mips2 -non_shared -g0 -no_AutoGnum -kp keep.txt g.u -ko linked || exit 1
$I/usplit -mips2 -o split -t st linked || exit 1
$I/umerge -Olimit 5000 -mips2 -EB -g0 -O3 $5 split -o merged -t st || exit 1
$I/uopt -G 0 -Olimit 5000 -mips2 -EB -g0 -O3 $6 merged opt -t st optlog || exit 1
$I/ugen -G 0 -mips2 -EB -g0 -O3 $7 opt -o gen -l out.s -t st -temp ugtmp || exit 1
$I/as1 -elf -G 0 -p0 -mips2 -EB -g0 -O3 -r4300_mul -Olimit 5000 gen -o m.o -t st || exit 1
echo "noalias directives: $(grep -c '\.noalias' out.s)"
cd ~/rush2049/scratch/frontier/w1b
IDO_DIR=$I python3 cand/objdiff.py $d/m.o "$3" | tail -1
rm -rf $d
