/* GENERATED ROM-aligned TU — segment 0x1f340 (rom/lib_1f340)
 * layout map e00424843f6bc10c711360c058d6ac0994494b0a03b7951830560602027a25aa; regenerate via `pipeline.layout convert`.
 * Slots are GLOBAL_ASM passthroughs until promoted; do not hand-edit
 * passthrough lines. */
#include "rom_tu.h"

/* PROMOTED 2026-10-04 — func_8001E740
 * Source:   cloud/matches/boot_tail/func_8001E740.c (in-repo, locked)
 * Flags:    -g0 -O2 -mips2 -G 0 -non_shared
 * Evidence: lock:cloud/matches/boot_tail/func_8001E740.c:func_8001E740 (score0); rom_tu.h context: cloud/work/boot_tail_promotion/context.jsonl
 * Gate:     full-ROM SHA-1 (promotion transaction)
 */
extern void *(*D_80038018)(unsigned int size, unsigned int mode);
void *func_8001E740(unsigned int size)
{
    return D_80038018(size, 0);
}

/* PROMOTED 2026-10-04 — func_8001E768
 * Source:   cloud/matches/boot_tail/func_8001E768.c (in-repo, locked)
 * Flags:    -g0 -O2 -mips2 -G 0 -non_shared
 * Evidence: lock:cloud/matches/boot_tail/func_8001E768.c:func_8001E768 (score0); rom_tu.h context: cloud/work/boot_tail_promotion/context.jsonl
 * Gate:     full-ROM SHA-1 (promotion transaction)
 */
extern void (*D_8003801C)(void *allocation);
void func_8001E768(void *allocation)
{
    D_8003801C(allocation);
}

#pragma GLOBAL_ASM("asm/us/nonmatchings/rom/lib_1f340/func_8001E790.s")
#pragma GLOBAL_ASM("asm/us/nonmatchings/rom/lib_1f340/func_8001E7BC.s")
#pragma GLOBAL_ASM("asm/us/nonmatchings/rom/lib_1f340/func_8001E864.s")
/* PROMOTED 2026-10-04 — func_8001E930
 * Source:   cloud/matches/boot_tail/func_8001E930.c (in-repo, locked)
 * Flags:    -g0 -O2 -mips2 -G 0 -non_shared
 * Evidence: lock:cloud/matches/boot_tail/func_8001E930.c:func_8001E930 (score0); rom_tu.h context: cloud/work/boot_tail_promotion/context.jsonl
 * Gate:     full-ROM SHA-1 (promotion transaction)
 */
void func_8001E930(unsigned int *time)
{
    *time *= 256U;
}

/* PROMOTED 2026-10-04 — func_8001E940
 * Source:   cloud/matches/boot_tail/func_8001E940.c (in-repo, locked)
 * Flags:    -g0 -O2 -mips2 -G 0 -non_shared
 * Evidence: lock:cloud/matches/boot_tail/func_8001E940.c:func_8001E940 (score0); rom_tu.h context: cloud/work/boot_tail_promotion/context.jsonl
 * Gate:     full-ROM SHA-1 (promotion transaction)
 */
extern u32 func_80019AA8(unsigned char *);
void func_8001E940(u32 *value, unsigned char *state)
{
    u32 rate;
    rate = func_80019AA8(state);
    *value = (((*value << 16) / rate) * 1000U) >> 5;
}

/* PROMOTED 2026-10-04 — func_8001E9A0
 * Source:   cloud/matches/boot_tail/func_8001E9A0.c (in-repo, locked)
 * Flags:    -g0 -O2 -mips2 -G 0 -non_shared
 * Evidence: lock:cloud/matches/boot_tail/func_8001E9A0.c:func_8001E9A0 (score0); rom_tu.h context: cloud/work/boot_tail_promotion/context.jsonl
 * Gate:     full-ROM SHA-1 (promotion transaction)
 */
unsigned int func_8001E9A0(unsigned int time)
{
    return time / 256U;
}

