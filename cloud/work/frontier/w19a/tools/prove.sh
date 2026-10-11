#!/bin/sh
# w19a full proof (builder, scratch root): IDO 7.1 ctx rows + LOCK/TU check for every lane/sources/*.c;
# TU edits alone == original; all sources spliced (promote_batch --prefix-dedup replica) into the
# edited TUs == original (IDO 7.1 both sides).
set -u
S=lane/sources
r=""; for f in $(ls $S | sed "s/\.c$//"); do r="$r $f=$S/$f.c"; done
python3 cloud/work/boot_tail_promotion/context_check.py --compiler ido7.1 lane/ctx71_O2.jsonl $r | grep -v " ok$"
for f in $(ls $S | sed 's/\.c$//'); do sh lane/tools/sc.sh $S/$f.c $f --ctx lane/ctx71_O2.jsonl 2>/dev/null | grep -E "^(==|LOCK:|TU:|CTX)" | cut -c1-200 | tr '\n' ' '; echo; done
rm -rf ed spl; python3 lane/tools/tu_edits.py ed/src/rom >/dev/null
mkdir -p spl/src/rom; cp ed/src/rom/*.c spl/src/rom/
echo "== TU edits alone"; sh lane/tools/tusplice.sh $(ls ed/src/rom | sed 's/\.c$//')
rm -rf spl; mkdir -p spl/src/rom tusrc; rm -f tusrc/*.c; cp src/rom/*.c tusrc/; cp ed/src/rom/*.c tusrc/
TUSRC=tusrc python3 lane/tools/splice.py lane/ctx71_O2.jsonl spl/src/rom $(ls $S | sed 's/\.c$//') >/dev/null
echo "== all sources spliced (into the edited TUs)"; sh lane/tools/tusplice.sh $(ls spl/src/rom | sed 's/\.c$//')
