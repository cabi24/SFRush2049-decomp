#!/bin/sh
# w27s: compile ultralib (decompals, reference/repos/ultralib e24c836) VERSION_J FINALROM sources with the project's
# IDO 5.3 at the locked flagset of the ROM TUs that hold them (all four TUs build at the default -g0 -O2, no
# opt override, compiler ido5.3 per rom_compilers.json) - w24t/w25a method.
#   lib_8440 io/sptask.c      @ 0x80007840 (_VirtualToPhysicalTask = project osViModeTableGet)
#   lib_e280 io/sprawdma.c    @ 0x8000D680 (__osSpRawStartDma = project __osSpDma)
#   lib_a050 io/controller.c  @ 0x80009450 (__osContGetInitData = __osContGetStatus, __osPackRequestData = __osContRamReset)
#   lib_f160 io/siacs.c       @ 0x8000E560 (__osSiCreateAccessQueue = osSiInit)
#   lib_f160 io/pfsgetstatus.c@ 0x8000E620 (__osPfsRequestOneChannel = __osContBuildRequest, __osPfsGetOneChannelData = __osContParseResponse)
# Usage (builder): sh prove.sh DIR   where DIR holds an ultralib/{include,src} copy.
set -e
D=$1
T=~/rush2049/cache/toolkits/796ae99a5cb7922e335e3afd87008753c573c82b45d04e8cfbce4f513cdbfbf5/ido
U=$D/ultralib
DEFS="-D_MIPS_SZLONG=32 -DF3DEX_GBI -DBUILD_VERSION=VERSION_J -DBUILD_VERSION_STRING=\"2.0J\" -DNDEBUG -D_FINALROM"
INC="-nostdinc -I $U/include -I $U/include/compiler/ido -I $U/include/PR"
cd $D
for f in sptask sprawdma controller siacs pfsgetstatus; do
  $T/cc -c -Wab,-r4300_mul -Xcpluscomm -g0 -O2 -mips2 -G 0 -non_shared $DEFS $INC $U/src/io/$f.c -o $f.o
done
sha256sum sptask.o sprawdma.o controller.o siacs.o pfsgetstatus.o
