#!/bin/sh
# oracle.sh LABEL PROC "ENV=V ..." [udiff args]: instrumented uopt (w17f uopt_or: PRIO, SAVELOG, SPL) on snapshot
# st_LABEL for PROC, then stock ugen + as1; fetch object; aligned diff of PROC vs retail. Diagnostic only.
L=$1; P=$2; E=$3; shift 3
S=rush2049/scratch/frontier/w24e; I=rush2049/cache/toolkits/796ae99a5cb7922e335e3afd87008753c573c82b45d04e8cfbce4f513cdbfbf5/ido
ssh watchman2 "cd ~/$S/st_$L && cp st st.or && env W5D_PROC=$P CDX_PROC=$P CDX_LOG=1 $E ../bin/uopt_or -G 0 -Olimit 5000 -mips2 -EB -g0 -O3 merged opt.or -t st.or optlog 2>or.spl >/dev/null && ~/$I/ugen -G 0 -mips2 -EB -g0 -O3 opt.or -o opt.or.gen -t st.or -temp ugtmp >/dev/null 2>&1 && ~/$I/as1 -elf -G 0 -p0 -mips2 -EB -g0 -O3 -r4300_mul -Olimit 5000 opt.or.gen -o or.o -t st.or >/dev/null 2>&1" || exit 1
mkdir -p /home/cburnes/projects/rush2049-decomp/build/trace/w24e
scp -q watchman2:$S/st_$L/or.o /home/cburnes/projects/rush2049-decomp/build/trace/w24e/or_$L.o
cd /home/cburnes/projects/rush2049-decomp && TAG=w24e python3 cloud/work/frontier/tools/trace/udiff.py $P --obj build/trace/w24e/or_$L.o "$@"
