#!/bin/sh
# builder side: gtr.sh DIR [env...]  (DIR has group.c; keep list fixed) -> DIR/merged, st; trace to DIR/trace.txt
d=$1; shift
T=$HOME/rush2049/cache/toolkits/796ae99a5cb7922e335e3afd87008753c573c82b45d04e8cfbce4f513cdbfbf5/ido
cd $d || exit 1
[ -f keep.txt ] || exit 1
$T/cc -j -g0 -O3 -mips2 -G 0 -non_shared group.c 2>err.txt || { tail err.txt; exit 1; }
$T/uld -L/usr/lib/mips2/nonshared -_SYSTYPE_SVR4 -mips2 -non_shared -g0 -no_AutoGnum -kp keep.txt group.u -ko linked >/dev/null 2>&1
$T/usplit -mips2 -o split -t st linked
$T/umerge -Olimit 5000 -mips2 -EB -g0 -O3 split -o merged -t st
cp st st.run && env CDX_LOG=1 CDX_OUT=$PWD/trace.txt "$@" ../../uopt/uopt -G 0 -Olimit 5000 -mips2 -EB -g0 -O3 merged opt.x -t st.run optlog >/dev/null 2>&1
echo done
