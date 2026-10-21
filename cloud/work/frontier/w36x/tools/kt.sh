#!/bin/sh
# kt.sh LABEL FILE.s NAME IMG [ENV...] : assemble listing with the patched debug as1 (env e.g. AS1K=0), diff NAME vs retail
L=$1; f=$(realpath "$2"); n=$3; img=$4; shift 4
scp -q "$f" watchman2:rush2049/scratch/frontier/w36x/st_$L/k_in.s
ssh watchman2 "cd ~/rush2049/scratch/frontier/w36x/st_$L && I=../bin/ido; UG='-G 0 -mips2 -EB -g0 -O3'; AS='-elf -G 0 -p0 -mips2 -EB -g0 -O3 -r4300_mul -Olimit 5000'; \$I/as0 \$UG k_in.s -o k_in.b -t k_in.st >/dev/null 2>&1 && env $* ../as1dbg/as1 \$AS k_in.b -o k_in.o -t k_in.st >/dev/null 2>&1" 
scp -q watchman2:rush2049/scratch/frontier/w36x/st_$L/k_in.o /home/cburnes/projects/rush2049-decomp/build/trace/w36x/k_$L.o
cd /home/cburnes/projects/rush2049-decomp && python3 cloud/work/frontier/w36x/tools/dd.py x $n --img $img --ctx 0 --obj build/trace/w36x/k_$L.o 2>/dev/null | grep -v "0x803c$\|,0x0$\|0(at)$\|lui at,0x8039\|lui a1,0x803c\|addiu a1,a1,0\|addiu a1,a1,-31240\|addiu a1,a1,-31228" | head -${DN:-12}
