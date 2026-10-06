#!/bin/sh
# usage: force.sh LABEL NAME PROC FORCESPEC -- re-run uopt (instrumented, CDX_FORCE) + ugen + as1 on snapshot
# st_LABEL, fetch the object and print the aligned diff of NAME vs retail. FORCESPEC e.g. "p1:w70=c6,p1:w102=c5"
lab=$1; name=$2; proc=$3; spec=$4
ssh watchman2 "cd ~/rush2049/scratch/frontier/w8d/st_$lab && T=\$HOME/rush2049/cache/toolkits/796ae99a5cb7922e335e3afd87008753c573c82b45d04e8cfbce4f513cdbfbf5/ido && cp st st.run && CDX_PROC=$proc CDX_FORCE='$spec' ../uopt/uopt -G 0 -Olimit 5000 -mips2 -EB -g0 -O3 merged opt.f -t st.run optlog >uopt.flog 2>&1 && \$T/ugen -G 0 -mips2 -EB -g0 -O3 opt.f -o gen.f -t st.run -temp ugtmp >/dev/null 2>&1 && \$T/as1 -elf -G 0 -p0 -mips2 -EB -g0 -O3 -r4300_mul -Olimit 5000 gen.f -o f.o -t st.run >/dev/null 2>&1 || tail -5 uopt.flog"
scp -q watchman2:rush2049/scratch/frontier/w8d/st_$lab/f.o /tmp/claude-1000/-home-cburnes-projects-rush2049-decomp/80274e1e-9a2b-4e9a-b2ec-780e794c33c9/scratchpad/f_$lab.o
cd /home/cburnes/projects/rush2049-decomp && python3 cloud/work/frontier/w8d/tools/udiff.py $name --obj /tmp/claude-1000/-home-cburnes-projects-rush2049-decomp/80274e1e-9a2b-4e9a-b2ec-780e794c33c9/scratchpad/f_$lab.o
