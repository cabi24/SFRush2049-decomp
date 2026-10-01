/* GENERATED ROM-aligned TU — segment 0xe3f0 (rom/lib_e3f0)
 * layout map e4c8bac5e9bf096ee593ec7b78ced3e899125fcef25292ac5e47d0c1880b12f0; regenerate via `pipeline.layout convert`.
 * Slots are GLOBAL_ASM passthroughs until promoted; do not hand-edit
 * passthrough lines. */
#include "rom_tu.h"

/* PROMOTED 2026-10-01 — osPiWriteWord
 * Source:   cloud/work/static_C5/osPiWriteWord.c (in-repo, locked)
 * Flags:    -g0 -O2 -mips2 -G 0 -non_shared
 * Evidence: lock:cloud/work/static_C5/osPiWriteWord.c:osPiWriteWord (score0)
 * Gate:     full-ROM SHA-1 (promotion transaction)
 */
s32 osPiWriteWord(u32 devAddr, u32 data) { if (__osPiDeviceBusy()) return -1; *(volatile u32 *)(devAddr | 0xA0000000U) = data; return 0; }

