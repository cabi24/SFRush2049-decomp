#!/bin/sh
# ON THE BUILDER: tk71.sh SCR MODE SRC FN [args]   (SCR = scratch root: include/, tgt/FN.o, t71/ido = traced 7.1)
# Compiles the reduced TU of SRC around FN (tools.conveyor.seeds.context.reduced_tu, exactly what `lock add`
# compiles) with the traced IDO 7.1 in SCR/t71/ido and the LOCK flags (the SRC's first-line `/* flags: */`, else
# -g0 -O2 -mips2 -G 0 -non_shared) plus -Wab,-r4300_mul (FLAGS=... overrides everything). Work dir SCR/t71/run/FN.
#   cdx    SRC FN [K=V..]    colouring trace of FN (CDX_LOG=1 CDX_FN=FN CDX_DETAIL_WEB=all): raw [CDX] lines
#                            (+ [CUP] caller-save cost lines with CUPLOG=1)
#   force  SRC FN SPEC       CDX_FN=FN CDX_FORCE=SPEC -> f.o; forced/declined records, then the row diff vs tgt/FN.o
#   ugen   SRC FN            traced ugen (DKWB_UGEN_TRACE, SCHED=1 adds EMIT records) on cc -S: FN's records,
#                            then ==LISTING== and FN's ugen listing (fn.s)
#   as1    SRC FN            the real compile with as1 -R (scheduler DAG + picks, source line numbers) -> as1r.log;
#                            also FN's ugen listing fn1.s (cc -S) to edit for asm
#   asm    FILE.s FN         assemble an (edited) listing with the stock 7.1 passes, row diff vs tgt/FN.o
#   spill  SRC FN            SPLOG=1 run: f_spilltemps homes + local-area growth for FN
#   obj    SRC FN            plain compile (traced passes, tracing off) -> o.o, row diff vs tgt/FN.o
#   vars   SRC FN            FN's declared locals/params from a -g3 build's .mdebug: `OFFSET NAME KIND` (= var@OFF)
S=$1; M=$2; SRC=$3; FN=$4; shift 4
I=$S/t71/ido
TK=${TK71:-$HOME/rush2049/cache/toolkits/4586d230749de1bc8a64add52ae53db0ffde18a1eee76c8a73f3791fd98e2557}
cd $S || exit 1
D=$S/t71/run/$FN; mkdir -p $D
fl=$(head -1 "$SRC" | sed -n 's@/\* flags: \(.*\) \*/@\1@p'); fl=${fl:--g0 -O2 -mips2 -G 0 -non_shared}
case " $fl " in *r4300_mul*) ;; *) fl="$fl -Wab,-r4300_mul" ;; esac
fl=${FLAGS:-$fl}
INC="-I $S/include -I $S/include/PR -I $TK/shim"
red() { python3 -c "
import sys; sys.path[:0]=['$S']
from tools.conveyor.seeds import context as c
open('$D/r.c','w').write(c.reduced_tu(open('$SRC').read(),'$FN'))" || { echo "reduced_tu failed"; exit 1; }; }
rows() { mips-linux-gnu-objdump -dr -j .text "$1" | sed -n '/^ *[0-9a-f]*:/p' | sed 's/^ *[0-9a-f]*:\s*//' | sed 's/\s\+/ /g'; }
rdiff() { [ -f $S/tgt/$FN.o ] || { echo "(no $S/tgt/$FN.o to diff against)"; return; }
  rows $S/tgt/$FN.o > $D/t.rows; rows "$1" > $D/m.rows
  if [ -n "$DIFF" ]; then diff -y -W 140 $D/t.rows $D/m.rows | grep -n -E '\s[|]\s|\s<$|\s>\s'; fi
  echo "rows: want $(wc -l < $D/t.rows) got $(wc -l < $D/m.rows); differing rows $(diff -y -W 140 $D/t.rows $D/m.rows | grep -c -E '\s[|]\s|\s<$|\s>\s') (objdump -dr rows incl. relocations; $D/{t,m}.rows)"; }
case $M in
cdx) red; (cd $D && env CDX_LOG=1 CDX_FN=$FN CDX_DETAIL_WEB=all CDX_OUT=$D/d.txt "$@" $I/cc -c $fl $INC -o $D/c.o r.c) >$D/cc.log 2>&1 || { cat $D/cc.log; exit 1; }
     grep -v procindex $D/d.txt; grep -h '^\[CUP\]' $D/cc.log; true ;;
force) red; spec=$1
     (cd $D && CDX_LOG=1 CDX_FN=$FN CDX_FORCE="$spec" CDX_OUT=$D/force.cdx $I/cc -c $fl $INC -o $D/f.o r.c) >$D/cc.log 2>&1 || { echo "cc failed:"; tail -5 $D/cc.log; exit 1; }
     grep -h DKWB $D/cc.log | head -5
     grep -E 'force_declined|p[12]color .*forced=(-1|[0-9])|p[12]dec .*forced=-1' $D/force.cdx; rdiff $D/f.o ;;
ugen) red; (cd $D && env DKWB_UGEN_TRACE=1 DKWB_UGEN_SCHED=${SCHED:-} $I/cc -S $fl $INC r.c) 2>$D/ugen.trace >/dev/null || { tail -5 $D/ugen.trace; exit 1; }
     k=$(grep -P '^\t\.ent\t' $D/r.s | awk -v n="$FN" '$2==n{print NR-1; exit}'); [ -n "$k" ] || { echo "no $FN in listing"; exit 1; }
     echo "ugen proc ordinal $k (procs in listing: $(grep -cP '^\t\.ent\t' $D/r.s), PROC BEGIN records: $(grep -c 'DKWB-PROC BEGIN' $D/ugen.trace))"
     awk -v k=$k '/^DKWB-PROC BEGIN/{p=($3=="proc=" k)} p && /^DKWB-(FREELIST|EMIT)/' $D/ugen.trace
     awk "/^\t.ent\t$FN( |\$)/,/^\t.end\t$FN\$/" $D/r.s > $D/fn.s; echo ==LISTING==; cat $D/fn.s ;;
as1) red; (cd $D && $I/cc -c $fl $INC -o c.o r.c && $I/cc -c $fl $INC -Wb,-R -o cr.o r.c > as1r.log 2>&1 && $I/cc -S $fl $INC r.c) >$D/cc.log 2>&1 || { cat $D/cc.log; exit 1; }
     cmp -s $D/c.o $D/cr.o && echo "as1-identical (as1 -R vs plain, whole reduced TU)" || echo "WARNING: as1 -R object differs"
     np=$(grep -cP '^\t\.ent\t' $D/r.s); echo "procedures in the reduced TU: $np ($(grep -P '^\t\.ent\t' $D/r.s | awk '{print $2}' | tr '\n' ' '))"
     [ "$np" = 1 ] || echo "NOTE: the -R trace has no procedure delimiters; blocks of FN follow the procedures before it in listing order"
     { printf '\t.text\n\t.globl\t%s\n' $FN; awk "/^\t.ent\t$FN( |\$)/,/^\t.end\t$FN\$/" $D/r.s; } > $D/fn1.s
     echo "as1 trace: $D/as1r.log ($(wc -l < $D/as1r.log) lines; lineno = source line), listing $D/fn1.s" ;;
asm) cp "$SRC" $D/x.s; (cd $D && $TK/ido/cc -c -g0 -O2 -mips2 -G 0 -non_shared -Wab,-r4300_mul x.s -o asm.o) || exit 1; rdiff $D/asm.o ;;
spill) red; (cd $D && SPLOG=1 $I/cc -c $fl $INC -o $D/s.o r.c) 2>$D/spill.log >/dev/null
     grep -E " proc=$FN( |\$)" $D/spill.log ;;
vars) red; fg=$(echo " $fl " | sed 's/ -g[0-3]* / /g'); (cd $D && $I/cc -c -g3 $fg $INC -o $D/g3.o r.c) >$D/g3.log 2>&1 || { cat $D/g3.log; exit 1; }
     python3 $S/t71/mdvars.py $D/g3.o $FN ;;
obj) red; (cd $D && $I/cc -c $fl $INC -o $D/o.o r.c) || exit 1; rdiff $D/o.o ;;
*) sed -n 2,16p $0; exit 2 ;;
esac
