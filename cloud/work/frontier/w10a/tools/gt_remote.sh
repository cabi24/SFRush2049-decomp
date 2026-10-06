#!/bin/sh
# runs ON THE BUILDER in ~/rush2049/scratch/frontier/w10a: gt_remote.sh FILE.c KEEP,LIST LABEL PROC
# stage the -O3 group (cc -j, uld -kp, usplit, umerge) into st_LABEL (merged + st), then run the w5d uopt with
# the level-3 listing, W5D bit/regcand trace and the colouring trace for PROC; outputs list, w5d, cdx in st_LABEL.
TK=$HOME/rush2049/cache/toolkits/796ae99a5cb7922e335e3afd87008753c573c82b45d04e8cfbce4f513cdbfbf5
I=$TK/ido
W=$HOME/rush2049/scratch/frontier/w10a
f=$1; keep=$2; lab=$3; proc=$4
d=$W/st_$lab; rm -rf $d; mkdir -p $d; cp "$f" $d/g.c; cd $d
for k in $(echo $keep | tr , ' '); do echo $k >> keep.txt; done
$I/cc -j -g0 -O3 -mips2 -G 0 -non_shared g.c || exit 1
$I/uld -L/usr/lib/mips2/nonshared -_SYSTYPE_SVR4 -mips2 -non_shared -g0 -no_AutoGnum -kp keep.txt g.u -ko linked || exit 1
$I/usplit -mips2 -o split -t st linked || exit 1
$I/umerge -Olimit 5000 -mips2 -EB -g0 -O3 split -o merged -t st || exit 1
cp st st.run
W5D_LEVEL=3 W5D_PROC=$proc W5D_OUT=$d/w5d CDX_LOG=1 CDX_DETAIL_WEB=all CDX_OUT=$d/cdx $W/../w5d/uopt/uopt -G 0 -Olimit 5000 -mips2 -EB -g0 -O3 merged opt -t st.run -l list >uopt.log 2>&1
grep -v procindex cdx > cdx.p; mv cdx.p cdx
awk "/LOCAL OPTIMIZATION OF $proc\$/{p=1} p{print} /REEMISSION OF $proc\$/{exit}" list > list.p; mv list.p list
echo staged $d
