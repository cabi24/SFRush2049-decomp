#!/bin/sh
# usage: force.sh LABEL NAME PROC_ORDINAL FORCESPEC -- re-run the w5d uopt with CDX_FORCE on snapshot st_LABEL,
# then ugen + as1, and print the aligned diff of NAME against retail. FORCESPEC e.g. "p1:w80=s" or "p1:w70=c6".
# The ordinal is printed by pretrace (report header "colouring ordinal N").
lab=$1; name=$2; proc=$3; spec=$4
S=${TMPDIR:-/tmp}/w8b
ssh watchman2 "cd ~/rush2049/scratch/frontier/w8b/st_$lab && T=\$HOME/rush2049/cache/toolkits/796ae99a5cb7922e335e3afd87008753c573c82b45d04e8cfbce4f513cdbfbf5/ido && cp st st.run && CDX_PROC=$proc CDX_FORCE='$spec' CDX_LOG=1 CDX_OUT=\$PWD/force.cdx ../uopt/uopt -G 0 -Olimit 5000 -mips2 -EB -g0 -O3 merged opt.f -t st.run optlog >uopt.flog 2>&1 && \$T/ugen -G 0 -mips2 -EB -g0 -O3 opt.f -o gen.f -t st.run -temp ugtmp >/dev/null 2>&1 && \$T/as1 -elf -G 0 -p0 -mips2 -EB -g0 -O3 -r4300_mul -Olimit 5000 gen.f -o f.o -t st.run >/dev/null 2>&1 || tail -5 uopt.flog; grep force_declined force.cdx"
mkdir -p $S; scp -q watchman2:rush2049/scratch/frontier/w8b/st_$lab/f.o $S/f_$lab.o
cd /home/cburnes/projects/rush2049-decomp && python3 cloud/work/frontier/w3a/tools/udiff.py $name --obj $S/f_$lab.o
