#!/bin/bash
V=$1
T=~/rush2049/cache/toolkits/796ae99a5cb7922e335e3afd87008753c573c82b45d04e8cfbce4f513cdbfbf5/ido
cd ~/claude_scratch/ultralib; OUT=~/claude_scratch/ulobj/$V; ok=0; bad=0
for f in $(find src -name '*.s'); do
  o=$OUT/$(echo ${f%.s} | tr / _).o
  if $T/cc -c -Wab,-r4300_mul -G 0 -nostdinc -woff 516,649,838,712 -mips2 -o32 -non_shared -O1 \
     -D_MIPS_SZLONG=32 -DBUILD_VERSION=VERSION_$V -DNDEBUG -D_FINALROM -D_LANGUAGE_ASSEMBLY \
     -I include -I include/compiler/ido -I include/PR $f -o $o 2>/dev/null; then ok=$((ok+1)); else bad=$((bad+1)); fi
done
echo "$V asm ok=$ok bad=$bad"; cd ~/claude_scratch/ulobj && tar czf ul_$V.tgz $V
