#!/bin/sh
# ON THE BUILDER: tk.sh SCRDIR LABEL MODE [args]  -- every tracer works on the snapshot SCRDIR/st_LABEL (merged + st)
# and never modifies merged/st (each run copies st). The Pi wrappers (../*.sh) call this; it can also be run by hand.
#   ord   NAME                 globalcolor ordinal of procedure NAME (the CDX_PROC value)
#   cdx   NAME [ENV=V...]      colouring trace of NAME (CDX_LOG, CDX_DETAIL_WEB=all), raw [CDX] lines to stdout
#   all                        colouring trace of every procedure -> st_LABEL/all.txt (for pdiff)
#   force NAME SPEC            CDX_FORCE=SPEC on NAME, then stock ugen + as1 -> st_LABEL/f.o; prints force records
#   pre   NAME                 W5D_LEVEL=3 listing + W5D bit/regcand trace + colouring trace -> st_LABEL/{list,w5d,cdx}
#   nopre NAME BITS [SPEC]     PRE oracle (W5D_NOPRE, VEC=d|s|i|u|h) [+ CDX_FORCE], ugen + as1 -> st_LABEL/n.o
#   spill NAME                 SPLOG + TMPLOG run; prints NAME's AREA/SPILLTEMP/HOME/CONFL/GETTEMP and [TMP] lines
#   ugen  NAME                 stock uopt -> traced ugen (DKWB_UGEN_TRACE, SCHED=1 adds emit records): NAME's
#                              FREELIST/EMIT lines, then ==LISTING== and NAME's ugen -l listing (st_LABEL/fn.s)
#   as1   NAME                 NAME's ugen listing -> as0 -> traced as1 -R (scheduler DAG + picks) -> st_LABEL/as1r.log
#   asm   FILE.s               as0 + stock as1 on an (edited) listing -> st_LABEL/asm.o
#   gstage FILE.c KEEP,LIST    (LABEL created) single-file -O3 group: cc -j, uld -kp, usplit, umerge -> merged + st
S=$1; L=$2; M=$3; shift 3
I=$S/bin/ido; X=$S/bin
UO="-G 0 -Olimit 5000 -mips2 -EB -g0 -O3"; UG="-G 0 -mips2 -EB -g0 -O3"; AS="-elf -G 0 -p0 -mips2 -EB -g0 -O3 -r4300_mul -Olimit 5000"
D=$S/st_$L
if [ "$M" = gstage ]; then
  rm -rf $D; mkdir -p $D; cp "$1" $D/g.c; cd $D
  for k in $(echo $2 | tr , ' '); do echo $k >> keep.txt; done
  $I/cc -j -g0 -O3 -mips2 -G 0 -non_shared g.c >cc.log 2>&1 || { tail cc.log; exit 1; }
  $I/uld -L/usr/lib/mips2/nonshared -_SYSTYPE_SVR4 -mips2 -non_shared -g0 -no_AutoGnum -kp keep.txt g.u -ko linked >/dev/null 2>&1 || exit 1
  $I/usplit -mips2 -o split -t st linked && $I/umerge -Olimit 5000 -mips2 -EB -g0 -O3 split -o merged -t st || exit 1
  echo "staged $D"; exit 0
fi
cd $D || { echo "no snapshot $D"; exit 1; }
ord() { cp st st.o; W5D_PROC=$1 W5D_OUT=$PWD/ord.w5d $X/uopt $UO merged opt.o -t st.o optlog >/dev/null 2>&1
        o=$(grep -m1 "ordinal proc=$1 " ord.w5d | sed 's/.*cdx_proc=//'); [ -n "$o" ] || { echo "no procedure $1 in this snapshot" >&2; exit 1; }; echo $o; }
backend() { # $1 opt file, $2 st copy, $3 output object
  $I/ugen $UG $1 -o $1.gen -t $2 -temp ugtmp >/dev/null 2>&1 && $I/as1 $AS $1.gen -o $3 -t $2 >/dev/null 2>&1; }
case $M in
ord) ord $1 ;;
cdx) n=$1; shift; cp st st.run; env W5D_PROC=$n CDX_LOG=1 CDX_DETAIL_WEB=all CDX_OUT=$PWD/d.txt "$@" $X/uopt $UO merged opt.x -t st.run optlog >/dev/null 2>&1
     grep -v procindex d.txt ;;
all) cp st st.run; CDX_LOG=1 CDX_OUT=$PWD/all.txt $X/uopt $UO merged opt.x -t st.run optlog >/dev/null 2>&1
     grep -v procindex all.txt | grep -E 'p[12]dec' | awk '{print $4}' | sort | uniq -c | sort -k2 -V > counts.txt; wc -l < counts.txt ;;
force) o=$(ord $1) || exit 1; cp st st.f
     CDX_PROC=$o CDX_FORCE="$2" CDX_LOG=1 CDX_OUT=$PWD/force.cdx $X/uopt $UO merged opt.f -t st.f optlog >uopt.flog 2>&1 || { echo "uopt failed:"; tail -5 uopt.flog; exit 1; }
     backend opt.f st.f f.o || { echo "ugen/as1 failed"; exit 1; }
     echo "proc $1 ordinal $o"; grep -h -E 'DKWB' uopt.flog | head -5
     grep -E 'force_declined|p[12]color .*forced=(-1|[0-9])|p[12]dec .*forced=-1' force.cdx ;;
pre) cp st st.run; W5D_LEVEL=3 W5D_PROC=$1 W5D_OUT=$PWD/w5d CDX_LOG=1 CDX_DETAIL_WEB=all CDX_OUT=$PWD/cdx $X/uopt $UO merged opt.p -t st.run -l list >uopt.log 2>&1
     grep -v procindex cdx > cdx.p; mv cdx.p cdx
     awk "/LOCAL OPTIMIZATION OF $1\$/{p=1} p{print} /REEMISSION OF $1\$/{exit}" list > list.p; mv list.p list; echo "pre: $D/{list,w5d,cdx}" ;;
nopre) o=$(ord $1) || exit 1; F=""; [ -n "$3" ] && F="CDX_PROC=$o CDX_FORCE=$3"; cp st st.n
     env W5D_NOPRE_VEC=${VEC:-d} W5D_PROC=$1 W5D_NOPRE=$2 W5D_OUT=$PWD/nopre.w5d $F $X/uopt $UO merged opt.n -t st.n optlog >uopt.nlog 2>&1 &&
       backend opt.n st.n n.o || { echo FAILED; tail -5 uopt.nlog; exit 1; }
     grep nopre nopre.w5d ;;
spill) cp st st.run; SPLOG=1 TMPLOG=1 $X/uopt $UO merged opt.s -t st.run optlog 2>spill.log >/dev/null
     # [TMP] lines carry the f_procinit count, the SPLOG lines the name: keep the [TMP] blocks that contain NAME
     awk -v n="$1" '/\[TMP\] procinit/{if(hit)printf "%s",b; b=""; hit=0} {b=b $0 "\n"} $0 ~ (" proc=" n "( |$)"){hit=1} END{if(hit)printf "%s",b}' spill.log |
       grep -E "\[TMP\]|proc=$1( |\$)" ;;
ugen) [ -f opt.u ] || { cp st st.u; $I/uopt $UO merged opt.u -t st.u optlog >/dev/null 2>&1; }
     cp st.u st.g; env DKWB_UGEN_TRACE=1 DKWB_UGEN_SCHED=${SCHED:-} $X/ugen $UG opt.u -o gen.u -l out.s -t st.g -temp ugtmp 2>ugen.trace >/dev/null
     k=$(grep -P '^\t\.ent\t' out.s | awk -v n="$1" '$2==n{print NR-1; exit}'); [ -n "$k" ] || { echo "no $1 in listing"; exit 1; }
     echo "ugen proc ordinal $k (procs in listing: $(grep -cP '^\t\.ent\t' out.s), PROC BEGIN records: $(grep -c 'DKWB-PROC BEGIN' ugen.trace))"
     awk -v k=$k '/^DKWB-PROC BEGIN/{p=($3=="proc=" k)} p && /^DKWB-(FREELIST|EMIT)/' ugen.trace
     awk "/^\t.ent\t$1 /,/^\t.end\t$1\$/" out.s > fn.s; echo ==LISTING==; cat fn.s ;;
as1) [ -f opt.u ] || { cp st st.u; $I/uopt $UO merged opt.u -t st.u optlog >/dev/null 2>&1; }
     cp st.u st.g; $I/ugen $UG opt.u -o gen.l -l out.s -t st.g -temp ugtmp >/dev/null 2>&1
     { printf '\t.text\n\t.globl\t%s\n' $1; awk "/^\t.ent\t$1 /,/^\t.end\t$1\$/" out.s; } > fn1.s
     $I/as0 $UG fn1.s -o fn1.b -t fn1.st >/dev/null 2>&1 || { echo "as0 failed on fn1.s"; exit 1; }
     cp fn1.st fn1.st2; $I/as1 $AS fn1.b -o fn1.o -t fn1.st >/dev/null 2>&1; $X/as1 $AS -R fn1.b -o fn1r.o -t fn1.st2 > as1r.log 2>&1
     cmp -s fn1.o fn1r.o && echo "as1-identical (traced vs stock)"; echo "as1 trace: $D/as1r.log ($(wc -l < as1r.log) lines), object $D/fn1.o" ;;
asm) $I/as0 $UG "$1" -o asm.b -t asm.st >/dev/null 2>&1 && $I/as1 $AS asm.b -o asm.o -t asm.st >/dev/null 2>&1 && echo "assembled $D/asm.o" ;;
*) sed -n 2,17p $0; exit 2 ;;
esac
