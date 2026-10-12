#!/bin/sh
# ON THE BUILDER: build71.sh SCRDIR RELIDO  -- compile the traced 7.1 passes (called by install71.sh).
# SCRDIR/t71/src/{uopt71,ugen71}.trace.c come from install71.sh; as1.c and the libc are copied from SCRDIR/isr71.
set -e
S=$1; REL=$2; R=$S/isr71
cd $S/t71/src
cp $R/header.h $R/libc_impl.h $R/helpers.h $R/libc_impl.c $R/build/7.1/as1.c .
python3 ../patches/ecvt.patch.py libc_impl.c
C="nice -n 10 gcc -std=c11 -Os -fno-strict-aliasing -I."
V='-DPACKAGE_VERSION="v1.2" -DDATETIME="t71"'
$C "$V" -c -DIDO71 -o libc_impl.o libc_impl.c 2>/dev/null
$C -DPACKAGE_VERSION='"v1.2"' -DDATETIME='"t71"' -c -DIDO71 -o version_info.o $R/version_info.c
$C -o ../bin/uopt uopt71.trace.c libc_impl.o version_info.o -lm 2>/dev/null &
$C -o ../bin/ugen ugen71.trace.c libc_impl.o version_info.o -lm 2>/dev/null
wait
$C -o ../bin/as1 as1.c libc_impl.o version_info.o -lm 2>/dev/null
cd ..; rm -rf reb ido; cp -rL $REL reb; cp -rL $REL ido
cp $R/build/7.1/out/uopt $R/build/7.1/out/ugen $R/build/7.1/out/as1 reb/
cp bin/uopt bin/ugen bin/as1 ido/
{ echo "# traced IDO 7.1 passes, built $(date -u +%FT%TZ) by cloud/work/frontier/tools/trace71/install71.sh"
  echo "# uopt = v1.2 uopt.c + instrument71_uopt.py + spill71.py; ugen = ugen.c + instrument-ugen --emit-provenance + ugen71_addrs.py"
  echo "# as1 = as1.c unchanged; libc = v1.2 libc_impl.c + ecvt.patch.py; reb = release dir + unpatched rebuild"
  for f in $R/build/7.1/uopt.c $R/build/7.1/ugen.c $R/build/7.1/as1.c src/uopt71.trace.c src/ugen71.trace.c src/libc_impl.c; do echo "src $(basename $f) $(sha256sum < $f | cut -c1-64)"; done
  for f in ido/uopt ido/ugen ido/as1 reb/uopt reb/ugen reb/as1; do echo "bin $f $(sha256sum < $f | cut -c1-64)"; done
} > MANIFEST
cat MANIFEST
