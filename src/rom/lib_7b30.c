/* GENERATED ROM-aligned TU — segment 0x7b30 (rom/lib_7b30)
 * layout map 475462c8028fc6222ef113a85b0dcc2925168d5611633204e7314996a266c32e; regenerate via `pipeline.layout convert`.
 * Slots are GLOBAL_ASM passthroughs until promoted; do not hand-edit
 * passthrough lines. */
#include "rom_tu.h"

/* PROMOTED 2026-10-01 — osCreateThread
 * Source:   cloud/work/static_C3/osCreateThread.c (in-repo, locked)
 * Flags:    -g0 -O1 -mips2 -G 0 -non_shared
 * Evidence: lock:cloud/work/static_C3/osCreateThread.c:osCreateThread (score0)
 * Gate:     full-ROM SHA-1 (promotion transaction)
 */
void osCreateThread(OSThread* t, OSId id, void (*entry)(void*), void* arg, void* sp, OSPri p) {
    register u32 saveMask;
    OSIntMask mask;


    t->id = id;
    t->priority = p;
    t->next = NULL;
    t->queue = NULL;
    t->context.pc = (u32)entry;
    t->context.a0 = (s64)(s32)arg; 
    t->context.sp = (s64)(s32)sp - 16;
    t->context.ra = (s64)(s32)__osExceptionPanic;
    mask = OS_IM_ALL;
    t->context.sr = (mask & (SR_IMASK | SR_IE)) | SR_EXL;
    t->context.rcp = (mask & RCP_IMASK) >> RCP_IMASKSHIFT;
    t->context.fpcsr = FPCSR_FS | FPCSR_EV | FPCSR_RM_RN;
    t->fp = 0;
    t->state = OS_STATE_STOPPED;
    t->flags = 0;


    saveMask = __osDisableInt();
    t->tlnext = __osRunQueue;
    __osRunQueue = t;
    __osRestoreInt(saveMask);
}

