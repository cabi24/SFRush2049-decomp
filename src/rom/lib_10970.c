/* GENERATED ROM-aligned TU — segment 0x10970 (rom/lib_10970)
 * layout map 55adbeab7d845fa8e9122493979db512ce24bc2c909b5edcca8f75a46e85cab1; regenerate via `pipeline.layout convert`.
 * Slots are GLOBAL_ASM passthroughs until promoted; do not hand-edit
 * passthrough lines. */
#include "rom_tu.h"

/* PROMOTED 2026-10-11 — __osSiDeviceBusy
 * Source:   cloud/work/frontier/w16g/sources/__osSiDeviceBusy.c (in-repo, locked)
 * Flags:    -g0 -O2 -mips2 -G 0 -non_shared
 * Evidence: lock:cloud/work/frontier/w16g/sources/__osSiDeviceBusy.c:__osSiDeviceBusy (score0); rom_tu.h context: cloud/work/frontier/w16g/ctx/ctx_O2.jsonl
 * Gate:     full-ROM SHA-1 (promotion transaction)
 */
#define TRUE 1
#define FALSE 0
#define IO_READ(addr) (*(volatile u32 *)(addr))
#define SI_STATUS_REG 0xA4800018
#define SI_STATUS_DMA_BUSY (1 << 0)
#define SI_STATUS_RD_BUSY (1 << 1)
int __osSiDeviceBusy(void) {
    register u32 stat = IO_READ(SI_STATUS_REG);

    if (stat & (SI_STATUS_DMA_BUSY | SI_STATUS_RD_BUSY)) {
        return TRUE;
    } else {
        return FALSE;
    }
}

