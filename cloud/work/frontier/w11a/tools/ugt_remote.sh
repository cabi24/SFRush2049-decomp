#!/bin/sh
# runs ON THE BUILDER in ~/rush2049/scratch/frontier/w11a/cand: ugt_remote.sh file.c keepname -> traced ugen (free-list events) + listing
TK=$HOME/rush2049/cache/toolkits/796ae99a5cb7922e335e3afd87008753c573c82b45d04e8cfbce4f513cdbfbf5
I=$TK/ido
U=$HOME/rush2049/scratch/frontier/w11a/ugen/ugen
d=$(mktemp -d); cp "$1" $d/g.c; cd $d
for k in $(echo $2 | tr , ' '); do echo $k >> keep.txt; done
$I/cc -j -g0 -O3 -mips2 -G 0 -non_shared g.c || exit 1
$I/uld -L/usr/lib/mips2/nonshared -_SYSTYPE_SVR4 -mips2 -non_shared -g0 -no_AutoGnum -kp keep.txt g.u -ko linked || exit 1
$I/usplit -mips2 -o split -t st linked || exit 1
$I/umerge -Olimit 5000 -mips2 -EB -g0 -O3 split -o merged -t st || exit 1
$I/uopt -G 0 -Olimit 5000 -mips2 -EB -g0 -O3 merged opt -t st optlog || exit 1
DKWB_UGEN_TRACE=1 $U -G 0 -mips2 -EB -g0 -O3 opt -o gen -l out.s -t st -temp ugtmp > trace.txt 2>&1
cat trace.txt
echo ==LISTING==
cat out.s
cd /; rm -rf $d
