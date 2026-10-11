#!/bin/sh
# w17g: re-score every D10 boot-tail "nonmatch" source (cloud/work/boot_tail, synced to ./bt) against its
# reloc-aware inventory target with IDO 7.1 (tk71 = decompals ido-static-recomp v1.2 ido-7.1-recomp-linux,
# sha256 0d411696...; other toolkit parts from d4c39cbc) at -g0 -O2 -mips2 -G 0 -non_shared, LOCK step of
# w16g check.py. Builder, scratch root. Output: one line per function.
while read fn src fl; do
  r=$(TK=$PWD/tk71 python3 lane/check.py $src $fn $fn --flags "-g0 -O2 -mips2 -G 0 -non_shared" 2>&1 | grep '^LOCK:')
  echo "$fn $src O2 $r"
done < lane/bt_list.txt
