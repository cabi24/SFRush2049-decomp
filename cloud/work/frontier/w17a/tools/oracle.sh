#!/bin/sh
# oracle.sh LABEL BIN "ENV=V ..." [udiff args]: run a private instrumented uopt (bin/BIN) on snapshot st_LABEL with
# W5D_PROC=camera_play_script and the env, then stock ugen + as1; stderr -> st_LABEL/or.log (copied to
# build/trace/w17a/or_LABEL.log); diff camera_play_script. Diagnostic only (never match evidence). (from w16d)
L=$1; B=$2; E=$3; shift 3
S=rush2049/scratch/frontier/w17a; I=rush2049/cache/toolkits/796ae99a5cb7922e335e3afd87008753c573c82b45d04e8cfbce4f513cdbfbf5/ido
ssh watchman2 "cd ~/$S/st_$L && cp st st.or && env W5D_PROC=camera_play_script $E ../bin/$B -G 0 -Olimit 5000 -mips2 -EB -g0 -O3 merged opt.or -t st.or optlog 2>or.log >/dev/null && ~/$I/ugen -G 0 -mips2 -EB -g0 -O3 opt.or -o opt.or.gen -t st.or -temp ugtmp >/dev/null 2>&1 && ~/$I/as1 -elf -G 0 -p0 -mips2 -EB -g0 -O3 -r4300_mul -Olimit 5000 opt.or.gen -o or.o -t st.or >/dev/null 2>&1" || exit 1
mkdir -p build/trace/w17a
scp -q watchman2:$S/st_$L/or.o build/trace/w17a/or_$L.o
scp -q watchman2:$S/st_$L/or.log build/trace/w17a/or_$L.log
cd /home/cburnes/projects/rush2049-decomp && TAG=w17a python3 cloud/work/frontier/tools/trace/udiff.py camera_play_script --obj build/trace/w17a/or_$L.o "$@"
