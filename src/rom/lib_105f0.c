/* GENERATED ROM-aligned TU — segment 0x105f0 (rom/lib_105f0)
 * layout map d973bfdc756aaecf31e12ea7faba871aae38af42089f45217aed2975232c9c96; regenerate via `pipeline.layout convert`.
 * Slots are GLOBAL_ASM passthroughs until promoted; do not hand-edit
 * passthrough lines. */
#include "rom_tu.h"

/* PROMOTED 2026-10-11 — __osContAddressCrc
 * Source:   cloud/work/frontier/w16g/sources/__osContAddressCrc.c (in-repo, locked)
 * Flags:    -g0 -O2 -mips2 -G 0 -non_shared
 * Evidence: lock:cloud/work/frontier/w16g/sources/__osContAddressCrc.c:__osContAddressCrc (score0); rom_tu.h context: cloud/work/frontier/w16g/ctx/ctx_O2.jsonl
 * Gate:     full-ROM SHA-1 (promotion transaction)
 */
u8 __osContAddressCrc(u16 addr) {
    u32 temp = 0;
    u32 i = (1 << 10);

    do {
        temp <<= 1;

        if ((u32)addr & i) {
            if (temp & (1 << 5)) {
                temp ^= 0x15 - 1;
            } else {
                ++temp;
            }
        } else if (temp & (1 << 5)) {
            temp ^= 0x15;
        }

        i >>= 1;
    } while (i != 0);

    i = 5;

    do {
        temp <<= 1;
        if (temp & (1 << 5)) {
            temp ^= 0x15;
        }
    } while (--i != 0);

    return temp & ((1 << 5) - 1);
}

/* PROMOTED 2026-10-11 — __osContDataCrc
 * Source:   cloud/work/frontier/w16g/sources/__osContDataCrc.c (in-repo, locked)
 * Flags:    -g0 -O2 -mips2 -G 0 -non_shared
 * Evidence: lock:cloud/work/frontier/w16g/sources/__osContDataCrc.c:__osContDataCrc (score0); rom_tu.h context: cloud/work/frontier/w16g/ctx/ctx_O2.jsonl
 * Gate:     full-ROM SHA-1 (promotion transaction)
 */
u8 __osContDataCrc(u8* data) {
    u32 temp = 0;
    u32 i;
    u32 j;

    for (i = 32; i; --i) {
        for (j = (1 << (8 - 1)); j; j >>= 1) {
            temp <<= 1;
            if ((*data & j) != 0) {
                if ((temp & (1 << 8)) != 0) {
                    temp ^= 0x85 - 1;
                } else {
                    ++temp;
                }
            } else if (temp & (1 << 8)) {
                temp ^= 0x85;
            }
        }

        data++;
    }
    do {
        temp <<= 1;
        if (temp & (1 << 8)) {
            temp ^= 0x85;
        }
    } while (++i < 8);

    return temp;
}

