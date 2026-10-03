#!/bin/bash
# ulbuild2.sh VERSION OPT : every C file at one optimisation level
V=$1; OPTF=$2
T=~/rush2049/cache/toolkits/796ae99a5cb7922e335e3afd87008753c573c82b45d04e8cfbce4f513cdbfbf5/ido
cd ~/claude_scratch/ultralib; OUT=~/claude_scratch/ulobj/$V$OPTF; rm -rf $OUT; mkdir -p $OUT; ok=0
for f in $(find src -name '*.c'); do
  o=$OUT/$(echo ${f%.c} | tr / _).o
  $T/cc -c -Wab,-r4300_mul -G 0 -nostdinc -Xcpluscomm -woff 516,649,838,712 -mips2 -o32 -non_shared $OPTF \
     -D_MIPS_SZLONG=32 -DBUILD_VERSION=VERSION_$V -DBUILD_VERSION_STRING=\"2.0$V\" -DNDEBUG -D_FINALROM -DF3DEX_GBI_2 \
     -I include -I include/compiler/ido -I include/PR $f -o $o 2>/dev/null && ok=$((ok+1))
done
echo "$V $OPTF ok=$ok"; cd ~/claude_scratch/ulobj && tar czf ul_$V$OPTF.tgz $V$OPTF
