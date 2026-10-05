#!/bin/sh
# Build the w5d instrumented IDO 5.3 uopt on the builder (watchman2) in ~/rush2049/scratch/frontier/w7d/uopt.
# Source: the recompiled uopt.c already on the builder (ido-static-recomp build, the same file w3a used).
# Instrumentation: workbench globalcolor profile + w5d hooks (instrument_w5d.py); libc ecvt/fcvt made real.
set -e
REPO=/home/cburnes/projects/rush2049-decomp
H=$REPO/cloud/work/frontier/w7d/tools
S=${TMPDIR:-/tmp}/w7d_uopt.$$
mkdir -p $S
scp -q watchman2:rush2049/scratch/ci/tools/ido-static-recomp/build/uopt.c $S/
python3 $H/instrument_w5d.py $S/uopt.c $S/uopt.w5d.c
ssh watchman2 'mkdir -p ~/rush2049/scratch/frontier/w7d/uopt && cd ~/rush2049/scratch/frontier/w7d/uopt && R=~/rush2049/scratch/ci/tools/ido-static-recomp && cp $R/header.h $R/libc_impl.h $R/helpers.h $R/libc_impl.c $R/build/out/err.english.cc .'
scp -q $S/uopt.w5d.c $H/ecvt.patch.py watchman2:rush2049/scratch/frontier/w7d/uopt/
ssh watchman2 'cd ~/rush2049/scratch/frontier/w7d/uopt && python3 ecvt.patch.py libc_impl.c && gcc -c -std=c11 -Os -fno-strict-aliasing -I. -DIDO53 -o libc_impl.o libc_impl.c && gcc -std=c11 -Os -fno-strict-aliasing -I. -o uopt uopt.w5d.c libc_impl.o -lm && ls -la uopt'
rm -rf $S
