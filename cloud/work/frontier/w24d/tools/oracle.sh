#!/bin/sh
# oracle.sh LABEL "ENV=V ..." [BIN] -- [udiff args]: run a private instrumented uopt (default bin/uopt_ulr) on snapshot
# st_LABEL with W5D_PROC=entity_render_setup (FN env overrides) and the given env, then stock ugen + as1, fetch the
# object and diff FN against retail. Diagnostic only (never match evidence). Adapted from w16d/tools/oracle.sh.
L=$1; E=$2; B=${3:-uopt_ulr}; shift 3
FN=${FN:-entity_render_setup}
S=rush2049/scratch/frontier/w24d; I=rush2049/cache/toolkits/796ae99a5cb7922e335e3afd87008753c573c82b45d04e8cfbce4f513cdbfbf5/ido
ssh watchman2 "cd ~/$S/st_$L && cp st st.or && env W5D_PROC=$FN $E ../bin/$B -G 0 -Olimit 5000 -mips2 -EB -g0 -O3 merged opt.or -t st.or optlog 2>or.spl >/dev/null && ~/$I/ugen -G 0 -mips2 -EB -g0 -O3 opt.or -o opt.or.gen -t st.or -temp ugtmp >/dev/null 2>&1 && ~/$I/as1 -elf -G 0 -p0 -mips2 -EB -g0 -O3 -r4300_mul -Olimit 5000 opt.or.gen -o or.o -t st.or >/dev/null 2>&1" || exit 1
mkdir -p /home/cburnes/projects/rush2049-decomp/build/trace/w24d
scp -q watchman2:$S/st_$L/or.o /home/cburnes/projects/rush2049-decomp/build/trace/w24d/or_$L.o
cd /home/cburnes/projects/rush2049-decomp && TAG=w24d python3 cloud/work/frontier/tools/trace/udiff.py $FN --obj build/trace/w24d/or_$L.o "$@"
