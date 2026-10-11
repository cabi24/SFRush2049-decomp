#!/bin/sh
# mdis.sh LABEL: stock uopt/ugen/as1 on a gstage snapshot st_LABEL, fetch the object and disassemble (mini probes)
L=$1; S=rush2049/scratch/frontier/w20g; I=rush2049/cache/toolkits/796ae99a5cb7922e335e3afd87008753c573c82b45d04e8cfbce4f513cdbfbf5/ido
ssh watchman2 "cd ~/$S/st_$L && cp st st.m && ~/$I/uopt -G 0 -Olimit 5000 -mips2 -EB -g0 -O3 merged m.u -t st.m /dev/null >/dev/null 2>&1 && ~/$I/ugen -G 0 -mips2 -EB -g0 -O3 m.u -o m.gen -t st.m -temp ugtmp >/dev/null 2>&1 && ~/$I/as1 -elf -G 0 -p0 -mips2 -EB -g0 -O3 -r4300_mul -Olimit 5000 m.gen -o m.o -t st.m >/dev/null 2>&1" || exit 1
D=/tmp/claude-1000/-home-cburnes-projects-rush2049-decomp/e6d4d7af-aa0e-4f5a-98a8-2e25f3b6dd6b/scratchpad
scp -q watchman2:$S/st_$L/m.o $D/m_$L.o && (mips-linux-gnu-objdump -d $D/m_$L.o 2>/dev/null || mips64-linux-gnu-objdump -d $D/m_$L.o) | sed -n '/<f>:/,$p'
