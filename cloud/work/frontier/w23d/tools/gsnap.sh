#!/bin/sh
# usage: gsnap.sh GROUPDIR LABEL  (on builder, in scratch) -> st_LABEL/{merged,st}
set -e
cd ~/rush2049/scratch/frontier/w23d
T=$HOME/rush2049/cache/toolkits/796ae99a5cb7922e335e3afd87008753c573c82b45d04e8cfbce4f513cdbfbf5/ido
G=$1; L=$2
rm -rf gw_$L; mkdir gw_$L; cd gw_$L
FILES=$(python3 -c "import json;j=json.load(open('../$G/group.json'));print(' '.join(j['files']))")
python3 -c "import json;j=json.load(open('../$G/group.json'));open('keep.txt','w').write(''.join(k+'\n' for k in j['keep']))"
FLAGS=$(python3 -c "import json;j=json.load(open('../$G/group.json'));print(j['flags'])")
for f in $FILES; do cp ../$G/$f .; done
$T/cc -j $FLAGS $FILES
U=$(for f in $FILES; do echo ${f%.c}.u; done)
$T/uld -L/usr/lib/mips2/nonshared -_SYSTYPE_SVR4 -mips2 -non_shared -g0 -no_AutoGnum -kp keep.txt $U -ko linked
$T/usplit -mips2 -o split -t st linked
$T/umerge -Olimit 5000 -mips2 -EB -g0 -O3 split -o merged -t st
cd ..; rm -rf st_$L; mkdir st_$L; cp gw_$L/merged gw_$L/st st_$L/
echo ok
