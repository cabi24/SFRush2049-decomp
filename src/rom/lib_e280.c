/* GENERATED ROM-aligned TU — segment 0xe280 (rom/lib_e280)
 * layout map ebc1e027f387408e3a8ba6e95320bf9867e1f988bc0f34a4ae0fa810874d954a; regenerate via `pipeline.layout convert`.
 * Slots are GLOBAL_ASM passthroughs until promoted; do not hand-edit
 * passthrough lines. */
#include "rom_tu.h"

/* PROMOTED 2026-10-01 — __osSpDma
 * Source:   cloud/work/static_C5/__osSpDma.c (in-repo, locked)
 * Flags:    -g0 -O2 -mips2 -G 0 -non_shared
 * Evidence: lock:cloud/work/static_C5/__osSpDma.c:__osSpDma (score0)
 * Gate:     full-ROM SHA-1 (promotion transaction)
 */
s32 __osSpDma(s32 direction, u32 devAddr, void *dramAddr, u32 size) {
 if (__osSpDeviceBusy()) return -1;
 (*(volatile u32 *)0xA4040000) = devAddr;
 (*(volatile u32 *)0xA4040004) = osVirtualToPhysical(dramAddr);
 if (direction == 0) { (*(volatile u32 *)0xA404000C) = size - 1; }
 else { (*(volatile u32 *)0xA4040008) = size - 1; }
 return 0;
}

