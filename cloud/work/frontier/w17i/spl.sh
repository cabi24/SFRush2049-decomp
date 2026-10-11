#!/bin/sh
# spl.sh LABEL PROC ORD "ENV=V ..." : run split-instrumented uopt (bin/uopt_spl = w16d patch_spl + w16b splitpatch)
# on snapshot st_LABEL for PROC (globalcolor ordinal ORD), then stock ugen+as1 -> st_LABEL/or.o; prints split log.
L=$1; P=$2; O=$3; E=$4
S=rush2049/scratch/frontier/w17i; I=rush2049/cache/toolkits/796ae99a5cb7922e335e3afd87008753c573c82b45d04e8cfbce4f513cdbfbf5/ido
ssh watchman2 "cd ~/$S/st_$L && cp st st.or && env W5D_PROC=$P SPL=1 SPLITLOG=$O $E ../bin/uopt_spl -G 0 -Olimit 5000 -mips2 -EB -g0 -O3 merged opt.or -t st.or optlog 2>or.spl >/dev/null && ~/$I/ugen -G 0 -mips2 -EB -g0 -O3 opt.or -o opt.or.gen -t st.or -temp ugtmp >/dev/null 2>&1 && ~/$I/as1 -elf -G 0 -p0 -mips2 -EB -g0 -O3 -r4300_mul -Olimit 5000 opt.or.gen -o or.o -t st.or >/dev/null 2>&1 && grep -E '^\[SPL' or.spl" || exit 1
mkdir -p /home/cburnes/projects/rush2049-decomp/build/trace/w17i
scp -q watchman2:$S/st_$L/or.o /home/cburnes/projects/rush2049-decomp/build/trace/w17i/or_$L.o
