#!/bin/sh
# ON BUILDER in ~/rush2049/scratch/frontier/w6a: as1t_remote.sh LABEL -- on group snapshot st_LABEL (gt.sh): stock uopt, ugen -l,
# stock as1 and traced as1 -R (log as1r.log); reports whether the two objects are identical.
T=$HOME/rush2049/cache/toolkits/796ae99a5cb7922e335e3afd87008753c573c82b45d04e8cfbce4f513cdbfbf5/ido
cd st_$1 || exit 1
cp st st.a && $T/uopt -G 0 -Olimit 5000 -mips2 -EB -g0 -O3 merged opt.a -t st.a optlog >/dev/null 2>&1
$T/ugen -G 0 -mips2 -EB -g0 -O3 opt.a -o gen.a -l out.s -t st.a -temp ugtmp >/dev/null 2>&1
cp st.a st.b
$T/as1 -elf -G 0 -p0 -mips2 -EB -g0 -O3 -r4300_mul -Olimit 5000 gen.a -o a.o -t st.a >/dev/null 2>&1
../as1/as1 -elf -G 0 -p0 -mips2 -EB -g0 -O3 -r4300_mul -Olimit 5000 -R gen.a -o r.o -t st.b > as1r.log 2>&1
cmp a.o r.o && echo as1-identical; wc -l as1r.log
