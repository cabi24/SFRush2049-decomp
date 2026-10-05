#!/bin/sh
# ON BUILDER (cand/): o3v_remote.sh file.c keep1,keep2 -> umerge -v log (inlining decisions) for the -O3 whole-program pipeline
TK=$HOME/rush2049/cache/toolkits/796ae99a5cb7922e335e3afd87008753c573c82b45d04e8cfbce4f513cdbfbf5
I=$TK/ido
d=$(mktemp -d); cp "$1" $d/g.c; cd $d
for k in $(echo $2 | tr , ' '); do echo $k >> keep.txt; done
$I/cc -j -g0 -O3 -mips2 -G 0 -non_shared g.c || exit 1
$I/uld -L/usr/lib/mips2/nonshared -_SYSTYPE_SVR4 -mips2 -non_shared -g0 -no_AutoGnum -kp keep.txt g.u -ko linked || exit 1
$I/usplit -mips2 -o split -t st linked || exit 1
$I/umerge -v -Olimit 5000 -mips2 -EB -g0 -O3 split -o merged -t st 2>&1
cd /; rm -rf $d
