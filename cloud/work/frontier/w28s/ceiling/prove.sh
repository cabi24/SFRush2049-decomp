#!/bin/sh
# w28s: assemble/compile the ultralib (decompals e24c836, reference/repos/ultralib) sources of the boot segment's
# remaining assembly functions exactly as ultralib's IDO build does (makefiles/ido.mk, libultra_rom: -D_FINALROM
# -DNDEBUG -non_shared, ASFLAGS -c -Wab,-r4300_mul -G 0 -nostdinc, .s -O1 (src/libc/*.s -O2), -mips2 -o32
# (exceptasm.s -mips3 -32), io/ai.c -O2) with the project's IDO 5.3, for one libultra VERSION.
# Then on the Pi: python3 cloud/work/frontier/w24t/rename/compare.py OBJ VRAM  (whole .text vs the ROM).
# Usage (builder): sh prove.sh DIR [VERSION]   DIR holds an ultralib/{include,src} copy; objects land in DIR/<VERSION>/.
set -e
D=$1; V=${2:-J}
T=~/rush2049/cache/toolkits/796ae99a5cb7922e335e3afd87008753c573c82b45d04e8cfbce4f513cdbfbf5/ido
U=$D/ultralib
O=$D/$V; mkdir -p $O
DEFS="-D_MIPS_SZLONG=32 -DF3DEX_GBI -DBUILD_VERSION=VERSION_$V -DBUILD_VERSION_STRING=\"2.0$V\" -non_shared -DNDEBUG -D_FINALROM"
INC="-I $U/include -I $U/include/compiler/ido -I $U/include/PR"
AS="$T/cc -c -Wab,-r4300_mul -G 0 -nostdinc -woff 516,649,838,712"
for f in os/setintmask os/writebackdcacheall os/writebackdcache os/invalicache os/invaldcache os/interrupt \
         os/getcount os/setsr os/getsr os/setfpccsr os/unmaptlball os/maptlbrdb os/getcause os/setcompare \
         os/probetlb gu/sqrtf; do
  (cd $U/src/$(dirname $f) && $AS -mips2 -o32 $DEFS -O1 $(basename $f).s $INC -o $O/$(basename $f).o)
done
for f in libc/bzero libc/bcopy libc/bcmp; do
  (cd $U/src/$(dirname $f) && $AS -mips2 -o32 $DEFS -O2 $(basename $f).s $INC -o $O/$(basename $f).o)
done
(cd $U/src/os && $AS -mips3 -32 $DEFS -O1 exceptasm.s $INC -o $O/exceptasm.o)
(cd $U/src/io && $T/cc -c -Wab,-r4300_mul -G 0 -nostdinc -Xcpluscomm -fullwarn -woff 516,649,838,712 -mips2 -o32 \
   $DEFS -O2 ai.c $INC -o $O/ai.o)
cd $O && sha256sum *.o
