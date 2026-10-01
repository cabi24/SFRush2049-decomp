/* GENERATED ROM-aligned TU — segment 0xf0b0 (rom/lib_f0b0)
 * layout map a093a43a395ea1e267f0564d35bdb0680375bd1beb15d826039d62458ce14878; regenerate via `pipeline.layout convert`.
 * Slots are GLOBAL_ASM passthroughs until promoted; do not hand-edit
 * passthrough lines. */
#include "rom_tu.h"

/* PROMOTED 2026-10-01 — __osSiRawStartDma
 * Source:   cloud/work/static_C5/__osSiRawStartDma.c (in-repo, locked)
 * Flags:    -g0 -O2 -mips2 -G 0 -non_shared
 * Evidence: lock:cloud/work/static_C5/__osSiRawStartDma.c:__osSiRawStartDma (score0)
 * Gate:     full-ROM SHA-1 (promotion transaction)
 */
s32 __osSiRawStartDma(s32 arg0, void *arg1)
{
  if ((*(volatile u32 *)0xA4800018) & 3)
  {
    return -1;
  }
  if (arg0 == 1)
  {
    osWritebackDCache(arg1, 0x40);
  }
  (*(volatile u32 *)0xA4800000) = osVirtualToPhysical(arg1);
  if (arg0 == 0)
  {
    (*(volatile u32 *)0xA4800004) = 0x1FC007C0;
  }
  else
  {
    (*(volatile u32 *)0xA4800010) = 0x1FC007C0;
  }
  if (arg0 == 0)
  {
    osInvalDCache(arg1, 0x40);
  }
  return 0;
}

