/* GENERATED ROM-aligned TU — segment 0x5610 (rom/lib_5610)
 * layout map 5f28eb4dc56fe3d76b0707d5fd55277a08a82e5e5499a3e3974d597d0c80bee6; regenerate via `pipeline.layout convert`.
 * Slots are GLOBAL_ASM passthroughs until promoted; do not hand-edit
 * passthrough lines. */
#include "rom_tu.h"

#pragma GLOBAL_ASM("asm/us/nonmatchings/rom/lib_5610/inflate_io_wait.s")
#pragma GLOBAL_ASM("asm/us/nonmatchings/rom/lib_5610/lzss_decode.s")
/* PROMOTED 2026-10-01 — inflate_flush_window
 * Source:   cloud/work/static_C/inflate_flush_window.c (in-repo, locked)
 * Flags:    -g0 -O2 -mips2 -G 0 -non_shared
 * Evidence: lock:cloud/work/static_C/inflate_flush_window.c:inflate_flush_window (score0)
 * Gate:     full-ROM SHA-1 (promotion transaction)
 */
void inflate_flush_window(s32 arg0, s32 arg1) {
    gDisplayListHead = arg0;
    gDisplayListEnd = arg1;
    *(s32 *)&gDisplayListSize = 0;
}

/* PROMOTED 2026-09-24 — huft_alloc
 * Source:   work/auto/huft_alloc/matched.c (in-repo, locked)
 * Flags:    -g0 -O2 -mips2 -G 0 -non_shared
 * Evidence: lock:work/auto/huft_alloc/matched.c:huft_alloc (score0)
 * Gate:     full-ROM SHA-1 (promotion transaction)
 */
s32 huft_alloc(s32 arg0)
{
  int new_var;
  gDisplayListSize += arg0;
  new_var = gDisplayListSize;
  new_var = (new_var - arg0) + gDisplayListHead;
  if (1)
  {
  }
  return new_var;
}

#pragma GLOBAL_ASM("asm/us/nonmatchings/rom/lib_5610/huft_build.s")
#pragma GLOBAL_ASM("asm/us/nonmatchings/rom/lib_5610/inflate_free_window.s")
#pragma GLOBAL_ASM("asm/us/nonmatchings/rom/lib_5610/inflate_stored.s")
#pragma GLOBAL_ASM("asm/us/nonmatchings/rom/lib_5610/inflate_fixed.s")
#pragma GLOBAL_ASM("asm/us/nonmatchings/rom/lib_5610/inflate_dynamic.s")
#pragma GLOBAL_ASM("asm/us/nonmatchings/rom/lib_5610/inflate_block.s")
#pragma GLOBAL_ASM("asm/us/nonmatchings/rom/lib_5610/inflate_loop.s")
#pragma GLOBAL_ASM("asm/us/nonmatchings/rom/lib_5610/inflate_read_bits.s")
#pragma GLOBAL_ASM("asm/us/nonmatchings/rom/lib_5610/inflate_entry.s")
#pragma GLOBAL_ASM("asm/us/nonmatchings/rom/lib_5610/inflate_entry_alt.s")
