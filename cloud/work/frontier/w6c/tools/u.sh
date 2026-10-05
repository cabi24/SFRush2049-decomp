#!/bin/sh
# usage: u.sh CAND.c NAME [more NAMES] -- score in unit (tag w6c), aligned diff of first NAME (D=1 prints it)
c=$(realpath "$1"); shift
cd /home/cburnes/projects/rush2049-decomp
python3 -m tools.conveyor.pipeline.blob_unit --tag w6c score "$@" --with "$c" 2>&1 | grep -E "^  (EQUAL|DIFF|FAIL|[A-Z]+ )|equal;|rror" | head -8
python3 cloud/work/frontier/w6c/tools/udiff.py $1 --obj build/blob_unit/w6c/unit.o > /tmp/claude-1000/-home-cburnes-projects-rush2049-decomp/80274e1e-9a2b-4e9a-b2ec-780e794c33c9/scratchpad/ud_$1.txt 2>&1
echo "diff lines: $(grep -c '^|' /tmp/claude-1000/-home-cburnes-projects-rush2049-decomp/80274e1e-9a2b-4e9a-b2ec-780e794c33c9/scratchpad/ud_$1.txt)"
[ -n "$D" ] && cat /tmp/claude-1000/-home-cburnes-projects-rush2049-decomp/80274e1e-9a2b-4e9a-b2ec-780e794c33c9/scratchpad/ud_$1.txt
true
