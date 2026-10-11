#!/bin/sh
# oracle.sh LABEL "ENV=V ..." [udiff args]: w16d's oracle.sh for lane w17c / anim_state_update. Runs the private
# split-instrumented uopt (bin/uopt_spl from w16d patch_spl.py) on snapshot st_LABEL with the given env, then stock
# ugen + as1, prints the split lines, fetches the object and diffs. Diagnostic only (never match evidence).
L=$1; E=$2; shift 2
N=${N:-anim_state_update}
S=rush2049/scratch/frontier/w17c; I=rush2049/cache/toolkits/796ae99a5cb7922e335e3afd87008753c573c82b45d04e8cfbce4f513cdbfbf5/ido
mkdir -p /home/cburnes/projects/rush2049-decomp/build/trace/w17c
ssh watchman2 "cd ~/$S/st_$L && cp st st.or && env W5D_PROC=$N SPL=1 $E ../bin/uopt_spl -G 0 -Olimit 5000 -mips2 -EB -g0 -O3 merged opt.or -t st.or optlog 2>or.spl >/dev/null && ~/$I/ugen -G 0 -mips2 -EB -g0 -O3 opt.or -o opt.or.gen -t st.or -temp ugtmp >/dev/null 2>&1 && ~/$I/as1 -elf -G 0 -p0 -mips2 -EB -g0 -O3 -r4300_mul -Olimit 5000 opt.or.gen -o or.o -t st.or >/dev/null 2>&1" || exit 1
scp -q watchman2:$S/st_$L/or.spl /home/cburnes/projects/rush2049-decomp/build/trace/w17c/or_$L.spl
scp -q watchman2:$S/st_$L/or.o /home/cburnes/projects/rush2049-decomp/build/trace/w17c/or_$L.o
cd /home/cburnes/projects/rush2049-decomp && TAG=w17c python3 cloud/work/frontier/tools/trace/udiff.py $N --obj build/trace/w17c/or_$L.o "$@" | tail -1
