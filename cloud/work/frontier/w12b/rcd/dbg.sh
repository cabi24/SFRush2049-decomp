#!/bin/sh
# dbg.sh FILE.s [LEVEL] : as0 + debug as1 (AS1DBG) on a listing; prints the xbb debug log
f=$(realpath "$1"); scp -q "$f" watchman2:rush2049/scratch/frontier/w12b/st_rcd/dbg_in.s
ssh watchman2 "cd ~/rush2049/scratch/frontier/w12b/st_rcd && I=../bin/ido; UG='-G 0 -mips2 -EB -g0 -O3'; AS='-elf -G 0 -p0 -mips2 -EB -g0 -O3 -r4300_mul -Olimit 5000'; \$I/as0 \$UG dbg_in.s -o dbg_in.b -t dbg_in.st >/dev/null 2>&1 && AS1BB=1 AS1DBG=${2:-3} ../as1dbg/as1 \$AS dbg_in.b -o dbg_in.o -t dbg_in.st 2>&1"
