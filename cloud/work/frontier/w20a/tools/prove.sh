#!/bin/sh
# w20a full proof (builder, scratch root): IDO 7.1 context rows + LOCK/TU check for every lane/sources/*.c;
# all sources spliced together (promote_batch --prefix-dedup replica, tools/splice.py) into their TUs, built with
# IDO 7.1 through asm-processor and compared with the current TUs (tools/tusplice.sh).
set -u
S=lane/sources
r=""; for f in $(ls $S | sed "s/\.c$//"); do r="$r $f=$S/$f.c"; done
python3 cloud/work/boot_tail_promotion/context_check.py --compiler ido7.1 lane/ctx71_O2.jsonl $r
for f in $(ls $S | sed 's/\.c$//'); do sh lane/tools/sc.sh $S/$f.c $f --ctx lane/ctx71_O2.jsonl 2>/dev/null | grep -E "^(==|LOCK:|TU:|RESULT)" | cut -c1-200 | tr '\n' ' '; echo; done
rm -rf spl; mkdir -p spl/src/rom
python3 lane/tools/splice.py lane/ctx71_O2.jsonl spl/src/rom $(ls $S | sed 's/\.c$//')
echo "== all sources spliced"; sh lane/tools/tusplice.sh $(ls spl/src/rom | sed 's/\.c$//')
