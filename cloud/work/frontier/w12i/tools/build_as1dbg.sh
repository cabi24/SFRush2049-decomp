#!/bin/sh
# build_as1dbg.sh [SCRATCH]  (run on the Pi) -- builds a debug as1 on the builder in SCRATCH/as1dbg (default
# rush2049/scratch/frontier/w12i). Same recompiled as1.c + libc patches as the trace toolkit (ecvt, real printf), plus:
#   AS1DBG=N  sets as1's internal debug level (the word at 0x10030818, normally only set by an option): >=3 prints the
#             cross-basic-block (xbb) pass ("current bb N, weight = W, stalls = S", MOVETO/MOVEFROM), >=4 per-bb
#             weights, >=8 dominator/postdominator sets and moved-block dumps.
#   AS1BB=1   (as1_patch_bb.py) prints, at entry to the xbb transform (func_42aa0c), every bb: id, weight, stalls,
#             flags and predecessor ids.
# Neither switch changes the object (checked: cmp with stock as1 output).
set -e
S=${1:-rush2049/scratch/frontier/w12i}; H=$(cd "$(dirname "$0")" && pwd); T=$H/../../tools/trace
ssh watchman2 "mkdir -p ~/$S/as1dbg/patches"
scp -q $T/patches/ecvt.patch.py $T/patches/as1_printf.patch.py watchman2:$S/as1dbg/patches/
scp -q $H/as1_patch_bb.py watchman2:$S/as1dbg/
ssh watchman2 "set -e; R=~/rush2049/scratch/ci/tools/ido-static-recomp; cd ~/$S/as1dbg && cp \$R/header.h \$R/libc_impl.h \$R/helpers.h \$R/libc_impl.c \$R/build/as1.c . && python3 patches/ecvt.patch.py libc_impl.c && python3 patches/as1_printf.patch.py && python3 -c '
s=open(\"as1.c\").read(); old=\"int ret = f_main(mem, 0xffffff0);\"
s=s.replace(old,\"if (getenv(\\\"AS1DBG\\\")) MEM_U32(0x10030818) = atoi(getenv(\\\"AS1DBG\\\"));\\n\"+old)
open(\"as1.c\",\"w\").write(\"#include <stdlib.h>\\n\"+s)' && python3 as1_patch_bb.py && C='nice -n 10 gcc -std=c11 -Os -fno-strict-aliasing -I.' && \$C -c -DIDO53 -o libc_impl.o libc_impl.c && \$C -o as1 as1.c libc_impl.o -lm && ls -la as1"
