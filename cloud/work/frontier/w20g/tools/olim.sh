#!/bin/sh
# olim.sh LABEL OLIMIT [udiff args]: ORACLE ONLY. Copy the current unit stage of tag w20g, rerun uld..as1 with
# umerge -Olimit OLIMIT (stock 5000) so that every call site is inlined, then diff entity_spawn_init.
L=$1; O=$2; shift 2
S=rush2049/scratch/frontier/w20g; T=rush2049/cache/toolkits/796ae99a5cb7922e335e3afd87008753c573c82b45d04e8cfbce4f513cdbfbf5/ido
ssh watchman2 "rm -rf ~/$S/ol_$L && cp -r ~/rush2049/scratch/frontier/unit/w20g/stage ~/$S/ol_$L && cd ~/$S/ol_$L && rm -f linked* split* merged* opt* gen* unit.o && T=~/$T &&
 \$T/uld -L/usr/lib/mips2/nonshared -_SYSTYPE_SVR4 -mips2 -non_shared -g0 -no_AutoGnum -kp keep.txt \$(cat units.txt) -ko linked >uld.log 2>&1 &&
 \$T/usplit -mips2 -o split -t st linked >usplit.log 2>&1 && \$T/umerge -v -Olimit 5000 $O -mips2 -EB -g0 -O3 split -o merged -t st >umerge.log 2>&1 &&
 cp st st.pre && \$T/uopt -G 0 -Olimit 5000 -mips2 -EB -g0 -O3 merged opt -t st optlog >uopt.log 2>&1 && \$T/ugen -G 0 -mips2 -EB -g0 -O3 opt -o gen -t st -temp ugtmp >ugen.log 2>&1 &&
 \$T/as1 -elf -G 0 -p0 -mips2 -EB -g0 -O3 -r4300_mul -Olimit 5000 gen -o unit.o -t st >as1.log 2>&1" || { echo FAIL; exit 1; }
mkdir -p /home/cburnes/projects/rush2049-decomp/build/trace/w20g
scp -q watchman2:$S/ol_$L/unit.o /home/cburnes/projects/rush2049-decomp/build/trace/w20g/ol_$L.o
ssh watchman2 "cd ~/$S/ol_$L && awk '/^  entity_spawn_init/{f=1;next} /^  [a-z]/{f=0} f' umerge.log | sort | uniq -c"
cd /home/cburnes/projects/rush2049-decomp && TAG=w20g python3 cloud/work/frontier/tools/trace/udiff.py entity_spawn_init --obj build/trace/w20g/ol_$L.o "$@"
