#!/bin/sh
# olim.sh LABEL "UMERGE-OPTS" NAME [udiff args]: ORACLE ONLY (w20g's olim.sh retagged). Copy the current unit stage
# of tag w37b, rerun uld..as1 with extra umerge options (e.g. "-space_time 6"), then diff NAME.
L=$1; O=$2; N=$3; shift 3
S=rush2049/scratch/frontier/w37b; T=rush2049/cache/toolkits/796ae99a5cb7922e335e3afd87008753c573c82b45d04e8cfbce4f513cdbfbf5/ido
ssh watchman2 "rm -rf ~/$S/ol_$L && cp -r ~/rush2049/scratch/frontier/unit/w37b/stage ~/$S/ol_$L && cd ~/$S/ol_$L && rm -f linked* split* merged* opt* gen* unit.o && T=~/$T &&
 \$T/uld -L/usr/lib/mips2/nonshared -_SYSTYPE_SVR4 -mips2 -non_shared -g0 -no_AutoGnum -kp keep.txt \$(cat units.txt) -ko linked >uld.log 2>&1 &&
 \$T/usplit -mips2 -o split -t st linked >usplit.log 2>&1 && \$T/umerge -v -Olimit 5000 $O -mips2 -EB -g0 -O3 split -o merged -t st >umerge.log 2>&1 &&
 cp st st.pre && \$T/uopt -G 0 -Olimit 5000 -mips2 -EB -g0 -O3 merged opt -t st optlog >uopt.log 2>&1 && \$T/ugen -G 0 -mips2 -EB -g0 -O3 opt -o gen -t st -temp ugtmp >ugen.log 2>&1 &&
 \$T/as1 -elf -G 0 -p0 -mips2 -EB -g0 -O3 -r4300_mul -Olimit 5000 gen -o unit.o -t st >as1.log 2>&1" || { echo FAIL; exit 1; }
mkdir -p /home/cburnes/projects/rush2049-decomp/build/trace/w37b
scp -q watchman2:$S/ol_$L/unit.o /home/cburnes/projects/rush2049-decomp/build/trace/w37b/ol_$L.o
ssh watchman2 "cd ~/$S/ol_$L && awk '/^  $N\$/{f=1;next} /^  [a-zA-Z_]/{f=0} f' umerge.log | sort | uniq -c"
cd /home/cburnes/projects/rush2049-decomp && TAG=w37b python3 cloud/work/frontier/tools/trace/udiff.py $N --obj build/trace/w37b/ol_$L.o "$@"
