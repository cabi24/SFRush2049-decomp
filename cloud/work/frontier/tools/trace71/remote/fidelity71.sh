#!/bin/sh
# ON THE BUILDER: fidelity71.sh SCRDIR [TU ...]  -- prove the traced 7.1 passes change nothing.
# Compiles each boot-tail TU (default: all 18 IDO 7.1 segments) from SCRDIR (scratch root with src/rom, include)
# with the ROM TU flags under four compilers and compares the objects byte for byte:
#   rel   the pinned release 7.1 toolkit IDO (decompals v1.2 release binaries)
#   reb   SCRDIR/t71/reb: the release dir with uopt/ugen/as1 rebuilt by us from the v1.2 generated C (no patches)
#   off   SCRDIR/t71/ido: traced uopt/ugen/as1, every trace switch off
#   on    SCRDIR/t71/ido with every trace on: CDX_LOG=1 CDX_DETAIL_WEB=all (all procedures), SPLOG=1, CUPLOG=1, DKWB_UGEN_TRACE=1
#         DKWB_UGEN_SCHED=1, as1 -R (-Wb,-R) -- trace output discarded, volume counted
# Prints one IDENTICAL/DIFFERENT line per TU and comparison, then a summary.
S=$1; shift
REL=${REL:-$HOME/rush2049/cache/toolkits/4586d230749de1bc8a64add52ae53db0ffde18a1eee76c8a73f3791fd98e2557/ido}
TUS=${*:-"11050 113e0 11640 15930 15a10 16320 17dc0 1a660 1cf90 1ece0 1f340 1f5b0 207b0 21210 22300 257f0 25bb0 26ab0"}
FL="-G 0 -mips2 -O2 -non_shared -I include -I include/PR -D_LANGUAGE_C -Wab,-r4300_mul -Xcpluscomm -I src/rom"
cd $S || exit 1
D=$S/t71/fid; rm -rf $D; mkdir -p $D
same=0; diff=0
for t in $TUS; do
  f=src/rom/lib_$t.c
  $REL/cc -c $FL -o $D/$t.rel.o $f 2>/dev/null || { echo "rel cc failed on $f"; diff=$((diff+1)); continue; }
  $S/t71/reb/cc -c $FL -o $D/$t.reb.o $f 2>/dev/null
  $S/t71/ido/cc -c $FL -o $D/$t.off.o $f 2>/dev/null
  CDX_LOG=1 CDX_DETAIL_WEB=all CDX_OUT=$D/$t.cdx SPLOG=1 CUPLOG=1 DKWB_UGEN_TRACE=1 DKWB_UGEN_SCHED=1 \
    $S/t71/ido/cc -c $FL -Wb,-R -o $D/$t.on.o $f >$D/$t.as1r 2>$D/$t.ugen
  for k in reb off on; do
    if cmp -s $D/$t.rel.o $D/$t.$k.o; then same=$((same+1)); r=IDENTICAL; else diff=$((diff+1)); r=DIFFERENT; fi
    echo "$r lib_$t: rel vs $k"
  done
  echo "  lib_$t trace volume: cdx $(grep -c '^\[CDX\]' $D/$t.cdx) ugen $(grep -c '^DKWB' $D/$t.ugen) spill $(grep -c -E '^(AREA|SPILLTEMP|HOME|CONFL|SPILLWEB) ' $D/$t.ugen) cup $(grep -c '^\[CUP\]' $D/$t.ugen) as1 -R $(wc -l < $D/$t.as1r)"
done
echo "fidelity71: $same identical, $diff different ($(echo $TUS | wc -w) TUs x 3 comparisons against the release 7.1)"
