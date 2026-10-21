#!/bin/sh
# spl.sh LABEL PROC "ENV=V ..." IMG [-v]: run the split-instrumented uopt (bin/uopt_spl, from w16d patch_spl.py) on
# snapshot st_LABEL with W5D_PROC=PROC and env, then stock ugen + as1; print split decisions for PROC and score vs overlay.
L=$1; P=$2; E=$3; IMG=$4; shift 4
S=rush2049/scratch/frontier/w36j; I=rush2049/cache/toolkits/796ae99a5cb7922e335e3afd87008753c573c82b45d04e8cfbce4f513cdbfbf5/ido
ssh watchman2 "cd ~/$S/st_$L && cp st st.or && env W5D_PROC=$P SPL=1 $E ../bin/uopt_spl -G 0 -Olimit 5000 -mips2 -EB -g0 -O3 merged opt.or -t st.or optlog 2>or.spl >/dev/null && ~/$I/ugen -G 0 -mips2 -EB -g0 -O3 opt.or -o opt.or.gen -t st.or -temp ugtmp >/dev/null 2>&1 && ~/$I/as1 -elf -G 0 -p0 -mips2 -EB -g0 -O3 -r4300_mul -Olimit 5000 opt.or.gen -o or.o -t st.or >/dev/null 2>&1; grep -c '' or.spl" || exit 1
mkdir -p /home/cburnes/projects/rush2049-decomp/build/trace/w36j
scp -q watchman2:$S/st_$L/or.o /home/cburnes/projects/rush2049-decomp/build/trace/w36j/or_$L.o
scp -q watchman2:$S/st_$L/or.spl /home/cburnes/projects/rush2049-decomp/build/trace/w36j/or_$L.spl
python3 /home/cburnes/projects/rush2049-decomp/cloud/work/frontier/w36j/tools/oscore.py /home/cburnes/projects/rush2049-decomp/build/trace/w36j/or_$L.o $P $IMG "$@"
