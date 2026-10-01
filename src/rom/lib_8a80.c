/* GENERATED ROM-aligned TU — segment 0x8a80 (rom/lib_8a80)
 * layout map 1e8133363e5eb9eb046c27ef049bfec2e1237c977eb189b1379811d73fbcbae4; regenerate via `pipeline.layout convert`.
 * Slots are GLOBAL_ASM passthroughs until promoted; do not hand-edit
 * passthrough lines. */
#include "rom_tu.h"

#pragma GLOBAL_ASM("asm/us/nonmatchings/rom/lib_8a80/__osInitialize_common.s")
/* PROMOTED 2026-10-01 — __osPiReadDeviceType
 * Source:   cloud/work/static_C6/__osPiReadDeviceType.c (in-repo, locked)
 * Flags:    -g0 -O1 -mips2 -G 0 -non_shared
 * Evidence: lock:cloud/work/static_C6/__osPiReadDeviceType.c:__osPiReadDeviceType (score0)
 * Gate:     full-ROM SHA-1 (promotion transaction)
 */
void __osPiReadDeviceType(void) {
 gSpTaskFlags0=7;
 gSpTaskFlags1=*(volatile u32 *)0xa4600014;
 gSpTaskFlags4=*(volatile u32 *)0xa4600018;
 gSpTaskFlags2=*(volatile u32 *)0xa460001c;
 gSpTaskFlags3=*(volatile u32 *)0xa4600020;
 gSpTaskResultA=7;
 gSpTaskResultB=*(volatile u32 *)0xa4600024;
 gSpTaskResultE=*(volatile u32 *)0xa4600028;
 gSpTaskResultC=*(volatile u32 *)0xa460002c;
 gSpTaskResultD=*(volatile u32 *)0xa4600030;
}

