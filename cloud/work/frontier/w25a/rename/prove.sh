#!/bin/sh
# w25a: compile ultralib (decompals, reference/repos/ultralib) VERSION_J FINALROM sources with the
# project's IDO 5.3 and the locked flagsets of the ROM TUs that hold them (w24t method).
#   lib_cc50 (os/timerintr.c):  -g0 -O1 -mips2 -G 0 -non_shared   (data: __osCurrentTime/__osBaseCounter/__osViIntrCount)
#   lib_f700 (io/contpfs.c):    -g0 -O2 -mips2 -G 0 -non_shared   (__osRepairPackId/__osCheckPackId)
#   lib_a810 (io/pfsinitpak.c): -g0 -O2 -mips2 -G 0 -non_shared   (caller of both)
# Usage (builder): sh prove.sh DIR   where DIR holds ultralib/{include,src} copies.
set -e
D=$1
T=~/rush2049/cache/toolkits/796ae99a5cb7922e335e3afd87008753c573c82b45d04e8cfbce4f513cdbfbf5/ido
U=$D/ultralib
DEFS="-D_MIPS_SZLONG=32 -DF3DEX_GBI -DBUILD_VERSION=VERSION_J -DBUILD_VERSION_STRING=\"2.0J\" -DNDEBUG -D_FINALROM"
INC="-nostdinc -I $U/include -I $U/include/compiler/ido -I $U/include/PR"
cd $D
$T/cc -c -Wab,-r4300_mul -Xcpluscomm -g0 -O1 -mips2 -G 0 -non_shared $DEFS $INC $U/src/os/timerintr.c -o timerintr.o
$T/cc -c -Wab,-r4300_mul -Xcpluscomm -g0 -O2 -mips2 -G 0 -non_shared $DEFS $INC $U/src/io/contpfs.c -o contpfs.o
$T/cc -c -Wab,-r4300_mul -Xcpluscomm -g0 -O2 -mips2 -G 0 -non_shared $DEFS $INC $U/src/io/pfsinitpak.c -o pfsinitpak.o
sha256sum timerintr.o contpfs.o pfsinitpak.o
