#!/bin/sh
# w19b full proof (builder, scratch root; lane dir synced to ./lane): ctx rows + check.py for all sources, then
# all lane/sources spliced (promote_batch --prefix-dedup replica, lane/tools/splice.py) into a scratch copy of
# the CURRENT src/rom TUs == the current TUs (IDO 7.1 both sides, asm-processor like the Makefile ROM-TU rule).
set -u
sh lane/tools/run.sh
rm -rf spl; mkdir -p spl/src/rom
python3 lane/tools/splice.py ${CTX:-lane/ctx71_O2.jsonl} spl/src/rom $(ls ${S:-lane/sources} | sed 's/\.c$//') >/dev/null
echo "== all sources spliced (into the current TUs)"; sh lane/tools/tusplice.sh $(ls spl/src/rom | sed 's/\.c$//')
