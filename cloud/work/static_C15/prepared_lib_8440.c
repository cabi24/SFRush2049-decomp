/* GENERATED ROM-aligned TU — segment 0x8440 (rom/lib_8440)
 * layout map 613434100350849012f376728964041bd15fd845765c6bfb3b8ea8a529605a8d; regenerate via `pipeline.layout convert`.
 * Slots are GLOBAL_ASM passthroughs until promoted; do not hand-edit
 * passthrough lines. */
#include "rom_tu.h"

/* Canonical SDK SP-task context; keep shared IO_READ behavior unchanged. */
extern OSTask gViModeTempBuffer;
extern OSTask *osViModeTableGet(OSTask *);
extern void osViModeNtscLan1(OSTask *);
extern void __osSpSetStatus(u32);
extern s32 __osSpSetPc(u32);
extern s32 __osSpDma(s32, u32, void *, u32);
#define OS_TASK_LOADABLE 4
#define OS_YIELD_DATA_SIZE 0xC00
#undef IO_READ
#define IO_READ(addr) (*(volatile u32 *)((u32)(addr) | 0xA0000000))
#define _osVirtualToPhysical(ptr) if (ptr != NULL) { ptr = (void *)osVirtualToPhysical(ptr); } (void)0
#define SP_CLR_YIELD 0x200
#define SP_CLR_YIELDED 0x800
#define SP_CLR_TASKDONE 0x2000
#define SP_SET_INTR_BREAK 0x100
#define SP_IMEM_START 0x04001000

#pragma GLOBAL_ASM("asm/us/nonmatchings/rom/lib_8440/osViModeTableGet.s")
#pragma GLOBAL_ASM("asm/us/nonmatchings/rom/lib_8440/osViModeNtscLan1.s")
/* PROMOTED 2026-10-01 — osViModeNtscLpn1
 * Source:   cloud/work/static_C2/osViModeNtscLpn1.c (in-repo, locked)
 * Flags:    -g0 -O2 -mips2 -G 0 -non_shared
 * Evidence: lock:cloud/work/static_C2/osViModeNtscLpn1.c:osViModeNtscLpn1 (score0)
 * Gate:     full-ROM SHA-1 (promotion transaction)
 */
void osViModeNtscLpn1(s32 arg0) {
    if (__osSpDeviceBusy() != 0) {
        do {

        } while (__osSpDeviceBusy() != 0);
    }
    __osSpSetStatus(0x125U);
}

