#!/bin/sh
# usage: [ORD=n] gforce.sh LABEL NAME FORCESPEC [udiff args] -- re-run uopt (w5d build, CDX_FORCE) + ugen + as1 on group
# snapshot st_LABEL (from gt.sh) and print the aligned diff of NAME vs retail. ORD defaults to the ordinal in runs/LABEL/w5d.
R=/home/cburnes/projects/rush2049-decomp
lab=$1; name=$2; spec=$3; shift; shift; shift
ord=${ORD:-$(grep -o "cdx_proc=[0-9]*" $R/cloud/work/frontier/w8a/runs/$lab/w5d | head -1 | cut -d= -f2)}
ssh watchman2 "cd ~/rush2049/scratch/frontier/w8a/st_$lab && T=\$HOME/rush2049/cache/toolkits/796ae99a5cb7922e335e3afd87008753c573c82b45d04e8cfbce4f513cdbfbf5/ido && cp st st.run && CDX_PROC=$ord CDX_FORCE='$spec' CDX_LOG=1 CDX_OUT=\$PWD/force.cdx ../../w5d/uopt/uopt -G 0 -Olimit 5000 -mips2 -EB -g0 -O3 merged opt.f -t st.run optlog >uopt.flog 2>&1 && \$T/ugen -G 0 -mips2 -EB -g0 -O3 opt.f -o gen.f -t st.run -temp ugtmp >/dev/null 2>&1 && \$T/as1 -elf -G 0 -p0 -mips2 -EB -g0 -O3 -r4300_mul -Olimit 5000 gen.f -o f.o -t st.run >/dev/null 2>&1 || tail -5 uopt.flog; grep -c force_declined force.cdx"
S=/tmp/claude-1000/-home-cburnes-projects-rush2049-decomp/80274e1e-9a2b-4e9a-b2ec-780e794c33c9/scratchpad
scp -q watchman2:rush2049/scratch/frontier/w8a/st_$lab/f.o $S/f_$lab.o
cd $R && python3 cloud/work/frontier/w8a/tools/udiff.py $name --obj $S/f_$lab.o "$@"
