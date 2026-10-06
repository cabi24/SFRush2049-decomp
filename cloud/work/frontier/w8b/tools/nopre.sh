#!/bin/sh
# usage: nopre.sh LABEL NAME PROC BITS [CDXFORCE] -- oracle: re-run the w5d uopt on snapshot st_LABEL with
# W5D_NOPRE=BITS for procedure PROC (code motion's insert/delete cleared for those expression bits), optional
# CDX_FORCE (needs the colouring ordinal: export ORD=N), then ugen + as1; aligned diff of NAME against retail.
lab=$1; name=$2; proc=$3; bits=$4; force=$5
S=${TMPDIR:-/tmp}/w8b
F=""; [ -n "$force" ] && F="CDX_PROC=$ORD CDX_FORCE='$force'"
ssh watchman2 "cd ~/rush2049/scratch/frontier/w8b/st_$lab && T=\$HOME/rush2049/cache/toolkits/796ae99a5cb7922e335e3afd87008753c573c82b45d04e8cfbce4f513cdbfbf5/ido && cp st st.run && env W5D_NOPRE_VEC=${VEC:-d} W5D_PROC=$proc W5D_NOPRE=$bits W5D_OUT=\$PWD/nopre.w5d $F ../uopt/uopt -G 0 -Olimit 5000 -mips2 -EB -g0 -O3 merged opt.n -t st.run optlog >uopt.nlog 2>&1 && \$T/ugen -G 0 -mips2 -EB -g0 -O3 opt.n -o gen.n -t st.run -temp ugtmp >/dev/null 2>&1 && \$T/as1 -elf -G 0 -p0 -mips2 -EB -g0 -O3 -r4300_mul -Olimit 5000 gen.n -o n.o -t st.run >/dev/null 2>&1 || { echo FAILED; tail -5 uopt.nlog; }; grep nopre nopre.w5d"
mkdir -p $S; scp -q watchman2:rush2049/scratch/frontier/w8b/st_$lab/n.o $S/n_$lab.o
cd /home/cburnes/projects/rush2049-decomp && python3 cloud/work/frontier/w3a/tools/udiff.py $name --obj $S/n_$lab.o
