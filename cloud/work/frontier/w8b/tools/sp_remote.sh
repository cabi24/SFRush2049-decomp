#!/bin/sh
# ON BUILDER in ~/rush2049/scratch/frontier/w8b: sp_remote.sh FILE.c KEEP,LIST LABEL -- stage the group and run the
# area/spill-temp instrumented uopt (uopt3); stderr in st_LABEL/sp.log
TK=$HOME/rush2049/cache/toolkits/796ae99a5cb7922e335e3afd87008753c573c82b45d04e8cfbce4f513cdbfbf5
I=$TK/ido
W=$HOME/rush2049/scratch/frontier/w8b
f=$1; keep=$2; lab=$3
d=$W/st_$lab; rm -rf $d; mkdir -p $d; cp "$f" $d/g.c; cd $d
for k in $(echo $keep | tr , ' '); do echo $k >> keep.txt; done
$I/cc -j -g0 -O3 -mips2 -G 0 -non_shared g.c || exit 1
$I/uld -L/usr/lib/mips2/nonshared -_SYSTYPE_SVR4 -mips2 -non_shared -g0 -no_AutoGnum -kp keep.txt g.u -ko linked || exit 1
$I/usplit -mips2 -o split -t st linked || exit 1
$I/umerge -Olimit 5000 -mips2 -EB -g0 -O3 split -o merged -t st || exit 1
cp st st.run
$W/uopt3/uopt -G 0 -Olimit 5000 -mips2 -EB -g0 -O3 merged opt -t st.run 2>sp.log >/dev/null
echo done
