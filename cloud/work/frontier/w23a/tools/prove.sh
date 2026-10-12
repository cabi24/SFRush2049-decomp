#!/bin/sh
# w23a full proof (builder, scratch root; w21a recipe): IDO 7.1 context rows + LOCK/TU check (with and without
# -Wab,-r4300_mul) for every lane/sources/*.c; all sources spliced together (promote_batch --prefix-dedup replica,
# tools/splice.py) into their TUs, built with IDO 7.1 through asm-processor and compared with the current TUs.
set -u
S=cloud/work/frontier/w23a/sources
r=""; for f in $(ls $S | sed "s/\.c$//"); do r="$r $f=$S/$f.c"; done
python3 cloud/work/boot_tail_promotion/context_check.py --compiler ido7.1 cloud/work/frontier/w23a/ctx71_O2.jsonl $r
for f in $(ls $S | sed 's/\.c$//'); do
  sh lane/tools/sc.sh $S/$f.c $f --ctx cloud/work/frontier/w23a/ctx71_O2.jsonl 2>/dev/null | grep -E "^(==|LOCK:|TU:|RESULT)" | cut -c1-200 | tr '\n' ' '; echo
  echo "  with -Wab,-r4300_mul: $(sh lane/tools/sc.sh $S/$f.c $f --flags '-g0 -O2 -mips2 -G 0 -non_shared -Wab,-r4300_mul' 2>/dev/null | grep -E '^LOCK:')"
done
rm -rf spl; mkdir -p spl/src/rom
python3 lane/tools/splice.py cloud/work/frontier/w23a/ctx71_O2.jsonl spl/src/rom $(ls $S | sed 's/\.c$//')
echo "== all sources spliced"; sh lane/tools/tusplice.sh $(ls spl/src/rom | sed 's/\.c$//')
