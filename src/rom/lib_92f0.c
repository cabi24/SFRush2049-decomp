/* GENERATED ROM-aligned TU — segment 0x92f0 (rom/lib_92f0)
 * layout map 370de43169ac2fb4dabb5672cfad215eab737c20fc24fd05fdf6c50952f27393; regenerate via `pipeline.layout convert`.
 * Slots are GLOBAL_ASM passthroughs until promoted; do not hand-edit
 * passthrough lines. */
#include "rom_tu.h"

/* PROMOTED 2026-09-24 — osWritebackDCache_full
 * Source:   work/auto/osWritebackDCache_full/matched.c (in-repo, locked)
 * Flags:    -g0 -O2 -mips2 -G 0 -non_shared
 * Evidence: lock:work/auto/osWritebackDCache_full/matched.c:osWritebackDCache_full (score0)
 * Gate:     full-ROM SHA-1 (promotion transaction)
 */
void osWritebackDCache_full(s32 arg0, s32 arg1, s32 arg2, s32 arg3)
{
  int new_var;
  if (!new_var)
  {
  }
  new_var = (int) (1 & 0xFF);
  if (new_var)
  {
  }
}

#pragma GLOBAL_ASM("asm/us/nonmatchings/rom/lib_92f0/osWritebackDCacheAll.s")
