#!/bin/sh
# usage: ub.sh CAND.c NAME "extra blob_unit args" -- like u.sh with extra args (e.g. --block FN)
c=$(realpath "$1"); n=$2; x=$3
cd /home/cburnes/projects/rush2049-decomp
python3 -m tools.conveyor.pipeline.blob_unit --tag w5c score $n $x --with "$c" 2>&1 | grep -E "^  (EQUAL|DIFF|FAIL|[A-Z]+ )|equal;|rror" | head -8
python3 cloud/work/frontier/w5c/tools/udiff.py $n --obj build/blob_unit/w5c/unit.o > /tmp/claude-1000/-home-cburnes-projects-rush2049-decomp/80274e1e-9a2b-4e9a-b2ec-780e794c33c9/scratchpad/ud_$n.txt 2>&1
echo "diff lines: $(grep -c '^|' /tmp/claude-1000/-home-cburnes-projects-rush2049-decomp/80274e1e-9a2b-4e9a-b2ec-780e794c33c9/scratchpad/ud_$n.txt)"
