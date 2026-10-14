#!/bin/sh
# acc.sh LABEL PROC "ENV=V ..." [udiff args]: run the force-accept split oracle (bin/uopt_acc = w16d patch_spl.py +
# w31a patch_acc.py) on snapshot st_LABEL for PROC with the given env (SPL_ACC=bb,.. SPL_SYM=sym SPL_REJ=..), then
# stock ugen + as1 and diff PROC against retail. Diagnostic only (never match evidence).
L=$1; P=$2; E=$3; shift 3
S=rush2049/scratch/frontier/w32a; I=rush2049/cache/toolkits/796ae99a5cb7922e335e3afd87008753c573c82b45d04e8cfbce4f513cdbfbf5/ido
ssh watchman2 "cd ~/$S/st_$L && cp st st.acc && env W5D_PROC=$P SPL=1 $E ../bin/uopt_acc -G 0 -Olimit 5000 -mips2 -EB -g0 -O3 merged opt.acc -t st.acc optlog 2>acc.spl >/dev/null && ~/$I/ugen -G 0 -mips2 -EB -g0 -O3 opt.acc -o opt.acc.gen -t st.acc -temp ugtmp >/dev/null 2>&1 && ~/$I/as1 -elf -G 0 -p0 -mips2 -EB -g0 -O3 -r4300_mul -Olimit 5000 opt.acc.gen -o acc.o -t st.acc >/dev/null 2>&1; echo rc=\$?" || exit 1
mkdir -p /home/cburnes/projects/rush2049-decomp/build/trace/w32a
scp -q watchman2:$S/st_$L/acc.o /home/cburnes/projects/rush2049-decomp/build/trace/w32a/acc_$L.o
scp -q watchman2:$S/st_$L/acc.spl /home/cburnes/projects/rush2049-decomp/build/trace/w32a/acc_$L.spl
cd /home/cburnes/projects/rush2049-decomp && TAG=w32a python3 cloud/work/frontier/tools/trace/udiff.py $P --obj build/trace/w32a/acc_$L.o "$@"
