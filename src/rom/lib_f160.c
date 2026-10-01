/* GENERATED ROM-aligned TU — segment 0xf160 (rom/lib_f160)
 * layout map d685f3d6dab9373376d3ef3c9803ad25e7e8685c23458b05fb9c1a12d42f1036; regenerate via `pipeline.layout convert`.
 * Slots are GLOBAL_ASM passthroughs until promoted; do not hand-edit
 * passthrough lines. */
#include "rom_tu.h"

/* PROMOTED 2026-10-01 — osSiInit
 * Source:   cloud/work/static_C/osSiInit.c (in-repo, locked)
 * Flags:    -g0 -O2 -mips2 -G 0 -non_shared
 * Evidence: lock:cloud/work/static_C/osSiInit.c:osSiInit (score0)
 * Gate:     full-ROM SHA-1 (promotion transaction)
 */
void osSiInit(void) {
    __osSiInitialized = 1;
    osCreateMesgQueue(&__osSiMesg, &__osSiMesgQueue, 1);
    osJamMesg(&__osSiMesg, NULL, 0);
}

/* PROMOTED 2026-10-01 — __osSiGetAccess
 * Source:   cloud/work/static_C2/__osSiGetAccess.c (in-repo, locked)
 * Flags:    -g0 -O2 -mips2 -G 0 -non_shared
 * Evidence: lock:cloud/work/static_C2/__osSiGetAccess.c:__osSiGetAccess (score0)
 * Gate:     full-ROM SHA-1 (promotion transaction)
 */
void __osSiGetAccess(void) {
    OSMesg dummyMesg;
    if (!__osSiInitialized) {
        osSiInit();
    }
    osRecvMesg(&__osSiMesg, &dummyMesg, OS_MESG_BLOCK);
}

/* PROMOTED 2026-10-01 — __osSiRelAccess
 * Source:   cloud/work/static_C2/__osSiRelAccess.c (in-repo, locked)
 * Flags:    -g0 -O2 -mips2 -G 0 -non_shared
 * Evidence: lock:cloud/work/static_C2/__osSiRelAccess.c:__osSiRelAccess (score0)
 * Gate:     full-ROM SHA-1 (promotion transaction)
 */
void __osSiRelAccess(void) {
    osJamMesg(&__osSiMesg, NULL, OS_MESG_NOBLOCK);
}

#pragma GLOBAL_ASM("asm/us/nonmatchings/rom/lib_f160/osContStartReadData.s")
#pragma GLOBAL_ASM("asm/us/nonmatchings/rom/lib_f160/__osContBuildRequest.s")
#pragma GLOBAL_ASM("asm/us/nonmatchings/rom/lib_f160/__osContParseResponse.s")
