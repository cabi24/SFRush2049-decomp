#!/bin/sh
# Build the instrumented IDO 5.3 uopt (workbench globalcolor profile) on the builder, in the w3a scratch.
# The generated recompiled uopt.c lives in ~/rush2049/scratch/ci/tools/ido-static-recomp/build/ (sha256 627eff8f...).
# Fidelity checked 2026-10-05: with tracing off its output on the whole game unit is byte-identical to the toolkit uopt.
set -e
S=/tmp/claude-1000/-home-cburnes-projects-rush2049-decomp/80274e1e-9a2b-4e9a-b2ec-780e794c33c9/scratchpad/uopt
mkdir -p $S
scp -q watchman2:rush2049/scratch/ci/tools/ido-static-recomp/build/uopt.c $S/
cd /home/cburnes/projects/rush2049-decomp
python3 tools/workbench.py instrument-uopt $S/uopt.c $S/uopt.traced.c --profile globalcolor --allow-unverified-source
ssh watchman2 'mkdir -p ~/rush2049/scratch/frontier/w3a/uopt && cd ~/rush2049/scratch/frontier/w3a/uopt && R=~/rush2049/scratch/ci/tools/ido-static-recomp && cp $R/header.h $R/libc_impl.h $R/helpers.h $R/libc_impl.c $R/build/out/err.english.cc .'
scp -q $S/uopt.traced.c watchman2:rush2049/scratch/frontier/w3a/uopt/
ssh watchman2 'cd ~/rush2049/scratch/frontier/w3a/uopt && gcc -c -std=c11 -Os -fno-strict-aliasing -I. -DIDO53 -o libc_impl.o libc_impl.c && gcc -std=c11 -Os -fno-strict-aliasing -I. -o uopt uopt.traced.c libc_impl.o -lm'
scp -q cloud/work/frontier/w3a/tools/tr.sh cloud/work/frontier/w3a/tools/sum.sh watchman2:rush2049/scratch/frontier/w3a/
