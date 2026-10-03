#!/bin/bash
# usage: ulbuild.sh VERSION ; compiles every src/**/*.c of ultralib with IDO 5.3 (libultra_rom flags)
V=$1
T=~/rush2049/cache/toolkits/796ae99a5cb7922e335e3afd87008753c573c82b45d04e8cfbce4f513cdbfbf5/ido
cd ~/claude_scratch/ultralib
OUT=~/claude_scratch/ulobj/$V; rm -rf $OUT; mkdir -p $OUT
ok=0; bad=0
for f in $(find src -name '*.c'); do
  d=$(dirname $f); top=$(echo $d | cut -d/ -f2)
  opt=-O2; case $top in os|debug|host) opt=-O1;; esac
  case $V in D|E|F|G|H|I) opt=-O1;; esac
  o=$OUT/$(echo ${f%.c} | tr / _).o
  if $T/cc -c -Wab,-r4300_mul -G 0 -nostdinc -Xcpluscomm -woff 516,649,838,712 -mips2 -o32 -non_shared $opt \
     -D_MIPS_SZLONG=32 -DBUILD_VERSION=VERSION_$V -DBUILD_VERSION_STRING=\"2.0$V\" -DNDEBUG -D_FINALROM -DF3DEX_GBI_2 \
     -I include -I include/compiler/ido -I include/PR $f -o $o 2>/dev/null; then ok=$((ok+1)); else bad=$((bad+1)); fi
done
echo "$V ok=$ok bad=$bad"
