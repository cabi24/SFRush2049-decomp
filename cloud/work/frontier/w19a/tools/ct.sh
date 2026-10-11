#!/bin/sh
# w19a (builder, scratch root): ctx row (IDO 7.1) + LOCK + TU check for SRC FN
src=$1; fn=$2; o=$(mktemp -p lane ctx.XXXX.jsonl)
python3 cloud/work/boot_tail_promotion/context_check.py --compiler ido7.1 $o $fn=$src | tail -1
sh lane/tools/sc.sh $src $fn --ctx $o 2>&1 | grep -E "^(LOCK|TU|CTX|RESULT)"; rm -f $o
