#!/bin/sh
# Pi side: rebuild the PRE-oracle uopt into the builder scratch t71/pre (needs t71/src/uopt71.trace.c from w22c's install)
set -e
S=rush2049/scratch/frontier/w25f; L=${TMPDIR:-/tmp}/p71b.$$; mkdir -p $L
scp -q watchman2:rush2049/scratch/frontier/w22c/t71/src/uopt71.trace.c $L/
python3 "$(dirname $0)/pre71.py" $L/uopt71.trace.c $L/uopt71.pre.c
scp -q $L/uopt71.pre.c watchman2:$S/t71/src/; rm -rf $L
ssh watchman2 "cd ~/$S/t71/src && for f in header.h helpers.h libc_impl.h libc_impl.o version_info.o; do [ -f \$f ] || cp ../../../w22c/t71/src/\$f .; done && nice -n 10 gcc -std=c11 -Os -fno-strict-aliasing -I. -o ../bin_pre_uopt uopt71.pre.c libc_impl.o version_info.o -lm 2>&1 | grep -i error; cd .. && rm -rf pre && cp -a ido pre && cp bin_pre_uopt pre/uopt && sha256sum pre/uopt"
