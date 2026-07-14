/* Auto-decompiled by mips_to_c (autodecomp). */
#include "rom_auto.h"

s32 osPiRawReadWord(s32 arg0, s32 arg1) {
    s32 sp1C;
    s32 temp_v0;

    osPiGetAccess();
    temp_v0 = osPiReadWord(arg0, arg1);
    sp1C = temp_v0;
    osPiReleaseAccess();
    return temp_v0;
}
