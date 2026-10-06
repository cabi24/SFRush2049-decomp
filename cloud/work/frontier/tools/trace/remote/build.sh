#!/bin/sh
# ON THE BUILDER: build.sh SCRDIR RECOMPDIR -- compile the traced passes (called by install.sh).
# SCRDIR/src/{uopt,ugen}.trace.c come from install.sh; as1.c and the libc are copied from RECOMPDIR.
set -e
S=$1; R=$2
cd $S/src
cp $R/header.h $R/libc_impl.h $R/helpers.h $R/libc_impl.c $R/build/as1.c .
cp $R/build/out/err.english.cc ../bin/
python3 ../patches/ecvt.patch.py libc_impl.c
python3 ../patches/as1_printf.patch.py
C="nice -n 10 gcc -std=c11 -Os -fno-strict-aliasing -I."
$C -c -DIDO53 -o libc_impl.o libc_impl.c
$C -o ../bin/uopt uopt.trace.c libc_impl.o -lm &
$C -o ../bin/ugen ugen.trace.c libc_impl.o -lm
wait
$C -o ../bin/as1 as1.c libc_impl.o -lm
cd ../bin
{
  echo "# traced IDO 5.3 passes, built $(date -u +%FT%TZ) by cloud/work/frontier/tools/trace/install.sh"
  echo "# uopt = uopt.c + instrument_w5d.py (globalcolor profile + w5d hooks) + patch_spill.py"
  echo "# ugen = ugen.c + workbench instrument-ugen --emit-provenance;  as1 = as1.c (unchanged)"
  echo "# libc = libc_impl.c + ecvt.patch.py + as1_printf.patch.py"
  for f in $R/build/uopt.c $R/build/ugen.c $R/build/as1.c $R/libc_impl.c; do echo "recomp $(basename $f) $(sha256sum < $f | cut -c1-64)"; done
  for f in ../src/uopt.trace.c ../src/ugen.trace.c ../src/libc_impl.c; do echo "src $(basename $f) $(sha256sum < $f | cut -c1-64)"; done
  for f in uopt ugen as1; do echo "bin $f $(sha256sum < $f | cut -c1-64)"; done
} > MANIFEST
cat MANIFEST
