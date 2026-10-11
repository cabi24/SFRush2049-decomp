#!/bin/sh
# w17k (builder, scratch root, after the 7.1 switch is in the tree): the promote_batch path for
# w17g's sources under IDO 7.1, without the pool and without touching any real lockfile.
#  1. context rows with the 7.1 ROM-TU compiler (context_check.py --compiler ido7.1);
#  2. simulated lockfiles (simlock.py): 7.1-bound entries, and legacy entries (must be refused);
#  3. promote_batch --dry-run (plan + refusals) for both, and --splice-out spl/ for the 7.1 one.
# The spliced TUs are then built in the full ROM by fullsplice.sh.
set -u
L=cloud/work/frontier/w17k
TK71=4586d230749de1bc8a64add52ae53db0ffde18a1eee76c8a73f3791fd98e2557
FNS=$(cat ${LIST:-$L/list58.txt})
SEGS="0x113e0 0x11640 0x15a10 0x17dc0 0x1a660 0x1cf90 0x1ece0 0x1f340 0x207b0 0x21210 0x22300 0x26ab0"
DEDUP=rom/lib_113e0,rom/lib_11640,rom/lib_15a10,rom/lib_17dc0,rom/lib_1a660,rom/lib_1cf90,rom/lib_1ece0,rom/lib_1f340,rom/lib_207b0,rom/lib_21210,rom/lib_22300,rom/lib_26ab0
ONLY=$(echo $FNS | tr ' ' ,)
r=""; for f in $FNS; do r="$r $f=cloud/work/frontier/w17g/sources/$f.c"; done
python3 cloud/work/boot_tail_promotion/context_check.py --compiler ido7.1 $L/ctx71_O2.jsonl $r | grep -vc " ok"
python3 $L/simlock.py $L/sim71.lock.json $TK71 ido7.1 $FNS
python3 $L/simlock.py $L/sim53.lock.json d4c39cbc85750cd02d3494f318c65e770bbf7d4b10aae137bcf5b5f149137c5b "" $FNS
echo "== legacy (5.3-proven) locks: every candidate must be refused"
python3 cloud/work/boot_tail_promotion/promote_batch.py $L/ctx71_O2.jsonl $SEGS --only $ONLY \
  --prefix-dedup $DEDUP --lockfile $L/sim53.lock.json --dry-run
echo "== 7.1-bound locks, 7.1 context rows"
rm -rf $L/spl
python3 cloud/work/boot_tail_promotion/promote_batch.py $L/ctx71_O2.jsonl $SEGS --only $ONLY \
  --prefix-dedup $DEDUP --lockfile $L/sim71.lock.json --splice-out $L/spl
