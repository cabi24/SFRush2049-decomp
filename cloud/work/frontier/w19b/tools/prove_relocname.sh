#!/bin/sh
# w19b relocname proof (builder, scratch root): ctx rows + LOCK vs the CURRENT inventory target (expected
# true=10/20 reloc_blind=0) and vs the re-split target tgt/<fn>_rs.o (built on the Pi by resplit_target.py from
# lane/reloc_addrs_w19b.txt); then lib_16320 with the six spliced vs lib_16320 built from asm re-split by the
# same entries (resplit_asm.py on a temporary copy of the scratch asm, restored afterwards).
set -u
export S=lane/relocname CTX=lane/ctx71_relocname.jsonl
sh lane/tools/run.sh
for f in $(ls $S | sed 's/\.c$//'); do
  echo "$f vs re-split target: $(TK=$HOME/rush2049/cache/toolkits/4586d230749de1bc8a64add52ae53db0ffde18a1eee76c8a73f3791fd98e2557 python3 lane/tools/check.py $S/$f.c $f ${f}_rs | grep LOCK:)"
done
rm -rf spl; mkdir -p spl/src/rom
python3 lane/tools/splice.py $CTX spl/src/rom $(ls $S | sed 's/\.c$//') >/dev/null
rm -rf asm.orig; cp -r asm asm.orig
python3 lane/tools/resplit_asm.py lane/reloc_addrs_w19b.txt $(for f in $(ls $S | sed 's/\.c$//'); do ls asm/us/nonmatchings/rom/*/$f.s; done)
echo "== six spliced vs re-split passthrough TU"; sh lane/tools/tusplice.sh $(ls spl/src/rom | sed 's/\.c$//')
rm -rf asm; mv asm.orig asm
