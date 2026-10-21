#!/bin/sh
# wt.sh LABEL FILE.s [DBGLEVEL]: as0 + instrumented as1 (AS1W=1 weight trace + AS1BB) on a listing in my st_LABEL
# (w36x/tools/wt.sh pointed at the w36w2 scratch; as1dbg built with w12b build_as1dbg.sh + w36x as1_patch_w.py)
f=$(realpath "$2"); scp -q "$f" watchman2:rush2049/scratch/frontier/w36w2/st_$1/dbg_in.s
ssh watchman2 "cd ~/rush2049/scratch/frontier/w36w2/st_$1 && I=../bin/ido; UG='-G 0 -mips2 -EB -g0 -O3'; AS='-elf -G 0 -p0 -mips2 -EB -g0 -O3 -r4300_mul -Olimit 5000'; \$I/as0 \$UG dbg_in.s -o dbg_in.b -t dbg_in.st >/dev/null 2>&1 && AS1W=1 AS1BB=1 AS1DBG=${3:-3} ../as1dbg/as1 \$AS dbg_in.b -o dbg_in.o -t dbg_in.st 2>&1"
