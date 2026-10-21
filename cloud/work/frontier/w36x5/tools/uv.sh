#!/bin/sh
# usage (builder): uv.sh FILE.c KEEP1[,KEEP2...]  -- single-file -O3 group with uopt -v; prints uopt's per-procedure size lines
I=$HOME/rush2049/cache/toolkits/796ae99a5cb7922e335e3afd87008753c573c82b45d04e8cfbce4f513cdbfbf5/ido
W=$HOME/rush2049/scratch/frontier/w36x5/uvwork; rm -rf $W; mkdir -p $W; cp "$1" $W/f.c; cd $W
echo "$2" | tr ',' '\n' > keep.txt
$I/cc -j -g0 -O3 -mips2 -G 0 -non_shared f.c >cc.log 2>&1 || { cat cc.log; exit 1; }
$I/uld -L/usr/lib/mips2/nonshared -_SYSTYPE_SVR4 -mips2 -non_shared -g0 -no_AutoGnum -kp keep.txt f.u -ko linked >/dev/null 2>&1
$I/usplit -mips2 -o split -t st linked >/dev/null 2>&1
$I/umerge -Olimit 5000 -mips2 -EB -g0 -O3 split -o merged -t st >/dev/null 2>&1
$I/uopt -v -G 0 -Olimit 5000 -mips2 -EB -g0 -O3 merged opt -t st optlog > uopt.v 2>&1
cat uopt.v | grep -i -E "${3:-.}" | head -60
