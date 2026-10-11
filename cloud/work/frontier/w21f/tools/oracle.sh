#!/bin/sh
# oracle.sh LABEL "ENV=V ..." [udiff args]: run the private instrumented uopt (bin/uopt_or = w16d patch_spl +
# w16b splitpatch + w21f prio_patch) on snapshot st_LABEL with W5D_PROC=entity_spawn_init and the given env
# (PRIO=10:0.9, SPL_SKIP=.., SPL_REJ=.. SPL_SYM=.., SPLITLOG=94), then stock ugen + as1, fetch the object and diff
# entity_spawn_init against retail. Diagnostic only (never match evidence). Logs: st_LABEL/or.spl
L=$1; E=$2; shift 2
S=rush2049/scratch/frontier/w21f; I=rush2049/cache/toolkits/796ae99a5cb7922e335e3afd87008753c573c82b45d04e8cfbce4f513cdbfbf5/ido
ssh watchman2 "cd ~/$S/st_$L && cp st st.or && env W5D_PROC=entity_spawn_init CDX_PROC=entity_spawn_init CDX_LOG=1 $E ../bin/uopt_or -G 0 -Olimit 5000 -mips2 -EB -g0 -O3 merged opt.or -t st.or optlog 2>or.spl >/dev/null && ~/$I/ugen -G 0 -mips2 -EB -g0 -O3 opt.or -o opt.or.gen -t st.or -temp ugtmp >/dev/null 2>&1 && ~/$I/as1 -elf -G 0 -p0 -mips2 -EB -g0 -O3 -r4300_mul -Olimit 5000 opt.or.gen -o or.o -t st.or >/dev/null 2>&1" || exit 1
mkdir -p /home/cburnes/projects/rush2049-decomp/build/trace/w21f
scp -q watchman2:$S/st_$L/or.o /home/cburnes/projects/rush2049-decomp/build/trace/w21f/or_$L.o
cd /home/cburnes/projects/rush2049-decomp && TAG=w21f python3 cloud/work/frontier/tools/trace/udiff.py entity_spawn_init --obj build/trace/w21f/or_$L.o "$@"
