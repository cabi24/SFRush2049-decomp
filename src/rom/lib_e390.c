/* GENERATED ROM-aligned TU — segment 0xe390 (rom/lib_e390)
 * layout map 40b69e4c9d4bf544dd9b2188f7584b73eb54f08addbde6cf820213d2c2a22ca5; regenerate via `pipeline.layout convert`.
 * Slots are GLOBAL_ASM passthroughs until promoted; do not hand-edit
 * passthrough lines. */
#include "rom_tu.h"

#pragma GLOBAL_ASM("asm/us/nonmatchings/rom/lib_e390/__osSetFpcCsr.s")
/* PROMOTED 2026-10-01 — osPiReadIo
 * Source:   cloud/work/static_C5/osPiReadIo.c (in-repo, locked)
 * Flags:    -g0 -O2 -mips2 -G 0 -non_shared
 * Evidence: lock:cloud/work/static_C5/osPiReadIo.c:osPiReadIo (score0)
 * Gate:     full-ROM SHA-1 (promotion transaction)
 */
s32 osPiReadIo(u32 devAddr, u32 *data) { if (__osPiDeviceBusy()) return -1; *data = *(volatile u32 *)(devAddr | 0xA0000000U); return 0; }

