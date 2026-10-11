#!/bin/sh
# w18c full proof (builder, scratch root): ctx rows + check.py for all sources; TU edits alone == original;
# all sources spliced into the edited TUs == original (IDO 7.1 both sides).
set -u
sh lane/tools/run.sh
rm -rf ed spl; python3 lane/tools/tu_edits.py ed/src/rom >/dev/null
mkdir -p spl/src/rom; cp ed/src/rom/*.c spl/src/rom/
echo "== TU edits alone"; sh lane/tools/tusplice.sh $(ls ed/src/rom | sed 's/\.c$//')
rm -rf spl; mkdir -p tusrc; rm -f tusrc/*.c; cp src/rom/*.c tusrc/; cp ed/src/rom/*.c tusrc/
TUSRC=tusrc python3 lane/tools/splice.py lane/ctx71_O2.jsonl spl/src/rom $(ls lane/sources | sed 's/\.c$//') >/dev/null
echo "== all sources spliced (into the edited TUs)"; sh lane/tools/tusplice.sh $(ls spl/src/rom | sed 's/\.c$//')
