#!/bin/sh
# ON THE BUILDER: fidelity.sh SCRDIR LABEL -- on snapshot SCRDIR/st_LABEL (whole unit), run each pass three ways:
# stock toolkit binary, traced binary with tracing off, traced binary with every trace on; all outputs must be
# byte-identical (uopt: opt + symbol table; ugen: gen + symbol table; as1: object). Also checks that the stock
# replay reproduces the unit's own unit.o. Trace volume is counted, not kept.
S=$1; L=$2; U=${3:-}
I=$S/bin/ido; X=$S/bin
UO="-G 0 -Olimit 5000 -mips2 -EB -g0 -O3"; UG="-G 0 -mips2 -EB -g0 -O3"; AS="-elf -G 0 -p0 -mips2 -EB -g0 -O3 -r4300_mul -Olimit 5000"
cd $S/st_$L || exit 1
rm -rf fid; mkdir fid; cd fid
ok=0; bad=0
same() { if cmp -s "$1" "$2"; then echo "IDENTICAL $3"; ok=$((ok+1)); else echo "DIFFERENT $3"; bad=$((bad+1)); fi; }
cp ../st st.a; $I/uopt $UO ../merged opt.stock -t st.a optlog >/dev/null 2>&1
cp ../st st.b; $X/uopt $UO ../merged opt.off -t st.b optlog >/dev/null 2>&1
cp ../st st.c; CDX_LOG=1 CDX_DETAIL_WEB=all CDX_OUT=$PWD/on.cdx W5D_LEVEL=3 W5D_OUT=$PWD/on.w5d TMPLOG=1 SPLOG=1 \
  $X/uopt $UO ../merged opt.on -t st.c -l on.list >/dev/null 2>on.sp
same opt.stock opt.off "uopt opt: stock vs traced (tracing off)"
same opt.stock opt.on  "uopt opt: stock vs traced (CDX_LOG all procs + W5D_LEVEL=3 listing + TMPLOG + SPLOG)"
same st.a st.c "uopt symbol table: stock vs traced (all on)"
echo "  trace volume: cdx $(wc -l < on.cdx) lines, w5d $(wc -l < on.w5d), listing $(wc -l < on.list), spill/tmp $(wc -l < on.sp)"
rm -f on.list
cp st.a s1; $I/ugen $UG opt.stock -o gen.stock -t s1 -temp ugtmp >/dev/null 2>&1
cp st.a s2; $X/ugen $UG opt.stock -o gen.off -t s2 -temp ugtmp >/dev/null 2>&1
cp st.a s3; n=$(DKWB_UGEN_TRACE=1 DKWB_UGEN_SCHED=1 $X/ugen $UG opt.stock -o gen.on -t s3 -temp ugtmp 2>&1 >/dev/null | grep -c -E '^DKWB-(FREELIST|EMIT-V1|PROC)')
same gen.stock gen.off "ugen gen: stock vs traced (tracing off)"
same gen.stock gen.on  "ugen gen: stock vs traced (DKWB_UGEN_TRACE=1 DKWB_UGEN_SCHED=1)"
same s1 s3 "ugen symbol table: stock vs traced (on)"
echo "  trace volume: $n FREELIST/EMIT/PROC records"
cp s1 t1; $I/as1 $AS gen.stock -o a.stock.o -t t1 >/dev/null 2>&1
cp s1 t2; $X/as1 $AS gen.stock -o a.off.o -t t2 >/dev/null 2>&1
cp s1 t3; n=$($X/as1 $AS -R gen.stock -o a.on.o -t t3 2>&1 | wc -l)
same a.stock.o a.off.o "as1 object: stock vs traced (no -R)"
same a.stock.o a.on.o  "as1 object: stock vs traced (-R scheduler trace)"
echo "  trace volume: as1 -R $n lines"
[ -n "$U" ] && same a.stock.o "$U" "stock replay of the snapshot vs the unit's unit.o"
sha1sum opt.stock gen.stock a.stock.o
echo "fidelity: $ok identical, $bad different"
cd ..; rm -rf fid
[ $bad = 0 ]
