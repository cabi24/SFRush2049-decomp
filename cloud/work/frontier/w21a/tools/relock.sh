#!/bin/sh
# w21a (builder, scratch root): score every locked function of SEG's TU (src/rom/SEG.c) under IDO 7.1 with FLAGS
# (default: lock flagset + -Wab,-r4300_mul) against its inventory target; prints one LOCK line per function.
TK=$HOME/rush2049/cache/toolkits/4586d230749de1bc8a64add52ae53db0ffde18a1eee76c8a73f3791fd98e2557
seg=$1; shift; FL=${FLAGS:--g0 -O2 -mips2 -G 0 -non_shared -Wab,-r4300_mul} ; FL="$FL -I src/rom"
for fn in "$@"; do printf '%s ' $fn; TK=$TK python3 lane/tools/check.py src/rom/$seg.c $fn $fn --flags "$FL" 2>&1 | grep -E '^LOCK:' || echo; done
