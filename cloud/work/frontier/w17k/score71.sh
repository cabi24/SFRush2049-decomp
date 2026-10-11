#!/bin/sh
# w17k (builder, scratch root): lock-style score of every function in LIST with the
# IDO 7.1 toolkit bundle tk71.tar.gz (sha 4586d230..., = d4c39cbc with ido/ swapped)
# unpacked at dl/tk71, using w16g check.py's LOCK step (= lock add's verify_entry:
# reduced TU, toolkit shim on the include path, jobs/scoring against the inventory
# target tgt/<fn>.o). Also the same with the pinned 5.3 toolkit for contrast.
# usage: sh cloud/work/frontier/w17k/score71.sh LIST
set -u
L=cloud/work/frontier/w17k
for fn in $(cat "$1"); do
  src=cloud/work/frontier/w17g/sources/$fn.c
  r71=$(TK=$PWD/dl/tk71 python3 $L/check.py $src $fn $fn --flags "-g0 -O2 -mips2 -G 0 -non_shared" 2>&1 | grep '^LOCK:')
  r53=$(TK=$PWD/dl/tk53x python3 $L/check.py $src $fn $fn --flags "-g0 -O2 -mips2 -G 0 -non_shared" 2>&1 | grep '^LOCK:')
  echo "$fn | 7.1 $r71 | 5.3 $r53"
done
