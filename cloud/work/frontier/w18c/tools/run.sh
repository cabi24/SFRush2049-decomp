#!/bin/sh
# w18c replay (builder, scratch root ~/rush2049/scratch/frontier/w18c; lane dir synced to ./lane).
# For every source in lane/sources (or $F): IDO 7.1 context rows (context_check.py --compiler ido7.1,
# -> lane/ctx71_O2.jsonl), then w16g check.py (LOCK = lock add's verify_entry with toolkit 4586d230;
# TU = real src/rom TU text before the slot + ctx preamble + body, ROM-TU flags), both against the
# reloc-aware inventory target tgt/FN.o.
set -u
S=lane/sources
F="${F:-$(ls $S | sed 's/\.c$//')}"
r=""; for f in $(ls $S | sed "s/\.c$//"); do r="$r $f=$S/$f.c"; done
python3 cloud/work/boot_tail_promotion/context_check.py --compiler ido7.1 lane/ctx71_O2.jsonl $r | grep -v " ok$"
for f in $F; do sh lane/tools/sc.sh $S/$f.c $f --ctx lane/ctx71_O2.jsonl | grep -E "^(==|LOCK:|TU:|CTX|RESULT)" | tr '\n' ' '; echo; done
