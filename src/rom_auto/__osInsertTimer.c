/* Auto-decompiled by mips_to_c (autodecomp). */
#include "rom_auto.h"

s32 __osInsertTimer(void) {
    if (__osPiMgrState.flag == 0) {
        return 0;
    }
    return __osPiMgrState.unk8;
}
