/* GENERATED ROM-aligned TU — segment 0xe7c0 (rom/lib_e7c0)
 * layout map 0a6559b9e67a367eae3e2798ff28a2cda0fa0eac96f868fb59552d670bc56f6c; regenerate via `pipeline.layout convert`.
 * Slots are GLOBAL_ASM passthroughs until promoted; do not hand-edit
 * passthrough lines. */
#include "rom_tu.h"

/* PROMOTED 2026-10-01 — osPiInit
 * Source:   cloud/work/static_C/osPiInit.c (in-repo, locked)
 * Flags:    -g0 -O2 -mips2 -G 0 -non_shared
 * Evidence: lock:cloud/work/static_C/osPiInit.c:osPiInit (score0)
 * Gate:     full-ROM SHA-1 (promotion transaction)
 */
void osPiInit(void) {
    __osPiInitialized = 1;
    osCreateMesgQueue(&__osPiMesgQueue, &__osPiMesg, 1);
    osJamMesg(&__osPiMesgQueue, NULL, 0);
}

/* PROMOTED 2026-10-01 — osPiGetAccess
 * Source:   cloud/work/static_C/osPiGetAccess.c (in-repo, locked)
 * Flags:    -g0 -O2 -mips2 -G 0 -non_shared
 * Evidence: lock:cloud/work/static_C/osPiGetAccess.c:osPiGetAccess (score0)
 * Gate:     full-ROM SHA-1 (promotion transaction)
 */
void osPiGetAccess(void) {
    void *sp1C;

    if (__osPiInitialized == 0) {
        osPiInit();
    }
    osRecvMesg(&__osPiMesgQueue, &sp1C, 1);
}

/* PROMOTED 2026-10-01 — osPiReleaseAccess
 * Source:   cloud/work/static_C/osPiReleaseAccess.c (in-repo, locked)
 * Flags:    -g0 -O2 -mips2 -G 0 -non_shared
 * Evidence: lock:cloud/work/static_C/osPiReleaseAccess.c:osPiReleaseAccess (score0)
 * Gate:     full-ROM SHA-1 (promotion transaction)
 */
void osPiReleaseAccess(void) {
    osJamMesg(&__osPiMesgQueue, NULL, 0);
}

/* PROMOTED 2026-10-01 — osPiReadWord
 * Source:   cloud/work/static_C8/osPiReadWord.c (in-repo, locked)
 * Flags:    -g0 -O2 -mips2 -G 0 -non_shared
 * Evidence: lock:cloud/work/static_C8/osPiReadWord.c:osPiReadWord (score0)
 * Gate:     full-ROM SHA-1 (promotion transaction)
 */
s32 osPiReadWord(u32 devAddr, u32* data) {
    register u32 stat;


    

    while ((stat = *(volatile u32*)0xA4600010) & 3);
    *data = (*(volatile u32*)((u32)osRomBase | devAddr | 0xA0000000));

    return 0;
}

