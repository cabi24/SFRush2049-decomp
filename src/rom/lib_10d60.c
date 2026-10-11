/* GENERATED ROM-aligned TU — segment 0x10d60 (rom/lib_10d60)
 * layout map 1eec946940dcffacc041b948c1e7606f7b20389466a7f58bb42d30e32fe0ed79; regenerate via `pipeline.layout convert`.
 * Slots are GLOBAL_ASM passthroughs until promoted; do not hand-edit
 * passthrough lines. */
#include "rom_tu.h"

#pragma GLOBAL_ASM("asm/us/nonmatchings/rom/lib_10d60/osPfsIsPlug.s")
#pragma GLOBAL_ASM("asm/us/nonmatchings/rom/lib_10d60/__osPfsRequestData.s")
/* PROMOTED 2026-10-11 — func_800103A0
 * Source:   cloud/work/frontier/w16g/sources/func_800103A0.c (in-repo, locked)
 * Flags:    -g0 -O2 -mips2 -G 0 -non_shared
 * Evidence: lock:cloud/work/frontier/w16g/sources/func_800103A0.c:func_800103A0 (score0); rom_tu.h context: cloud/work/frontier/w16g/ctx/ctx_O2.jsonl
 * Gate:     full-ROM SHA-1 (promotion transaction)
 */
#define CHNL_ERR(format) (((format).rxsize & 0xC0) >> 4)
extern u8 __osPfsBuffer[64];
extern u8 __osPfsRequestType2;
void func_800103A0(u8* pattern, OSContStatus* data) {
    u8* ptr;
    __OSContRequesFormat requestformat;
    int i;
    u8 bits = 0;

    ptr = (u8*)&__osPfsBuffer;

    for (i = 0; i < __osPfsRequestType2; i++, ptr += sizeof(requestformat), data++) {
        requestformat = *((__OSContRequesFormat*)ptr);
        data->errno = CHNL_ERR(requestformat);

        if (data->errno != 0) {
            continue;
        }

        data->type = ((requestformat.typel << 8) | requestformat.typeh);
        data->status = requestformat.status;
        bits |= (1 << i);
    }
    *pattern = bits;
}

