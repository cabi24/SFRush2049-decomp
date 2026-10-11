#include "PR/os_internal.h"
#ifdef T_DATACRC
u8 __osContDataCrc(u8* data) {
    u32 temp = 0; u32 i; u32 j;
    for (i = 32; i; --i) {
        for (j = (1 << 7); j; j >>= 1) {
            temp <<= 1;
            if ((*data & j) != 0) {
                if ((temp & (1 << 8)) != 0) { temp ^= 0x85 - 1; } else { ++temp; }
            } else if (temp & (1 << 8)) { temp ^= 0x85; }
        }
        data++;
    }
    do { temp <<= 1; if (temp & (1 << 8)) { temp ^= 0x85; } } while (++i < 8);
    return temp;
}
#endif
