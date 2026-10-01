/* GENERATED ROM-aligned TU — segment 0xd140 (rom/lib_d140)
 * layout map 53b8eb1084c219370f1dddec7c46660b8a168d988671bc8f55ec231a4bdd5b31; regenerate via `pipeline.layout convert`.
 * Slots are GLOBAL_ASM passthroughs until promoted; do not hand-edit
 * passthrough lines. */
#include "rom_tu.h"

/* PROMOTED 2026-10-01 — osViInit
 * Source:   cloud/work/static_C11/osViInit.c (in-repo, locked)
 * Flags:    -g0 -O2 -mips2 -G 0 -non_shared
 * Evidence: lock:cloud/work/static_C11/osViInit.c:osViInit (score0)
 * Gate:     full-ROM SHA-1 (promotion transaction)
 */
void osViInit(void) {
    bzero(__osViMode, 96);
    (*(__OSViContext **)&__osViModeInfo) = ((__OSViContext *)__osViMode);
    __osViContext = ((__OSViContext *)__osViModeTable);
    __osViContext->retraceCount = 1;
    (*(__OSViContext **)&__osViModeInfo)->retraceCount = 1;
    __osViContext->framep = (void*)K0BASE;
    (*(__OSViContext **)&__osViModeInfo)->framep = (void*)K0BASE;

    if (osTvType == OS_TV_TYPE_PAL) {
        __osViContext->modep = &__osViModePending;
    } else if (osTvType == OS_TV_TYPE_MPAL) {
        __osViContext->modep = &__osViModeNext;
    } else {
        __osViContext->modep = &__osViModeBuffer;
    }

    __osViContext->state = VI_STATE_BLACK;
    __osViContext->control = __osViContext->modep->comRegs.ctrl;

    while ((*(volatile u32 *)0xA4400010) > 10) { 
    }

    *(volatile u32 *)0xA4400000 = 0; 
    __osViSwapContext();
}

