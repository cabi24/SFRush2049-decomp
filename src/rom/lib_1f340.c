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

/* PROMOTED 2026-10-11 — func_8001E790
 * Source:   cloud/work/frontier/w17g/sources/func_8001E790.c (in-repo, locked)
 * Flags:    -g0 -O2 -mips2 -G 0 -non_shared
 * Evidence: lock:cloud/work/frontier/w17g/sources/func_8001E790.c:func_8001E790 (score0); rom_tu.h context: cloud/work/frontier/w17k/ctx71_O2.jsonl
 * Gate:     full-ROM SHA-1 (promotion transaction)
 */
extern unsigned int D_8002CC50;
unsigned short func_8001E790(void)
{
    D_8002CC50 *= 2822053219U;
    return D_8002CC50 >> 6;
}

#pragma GLOBAL_ASM("asm/us/nonmatchings/rom/lib_1f340/func_8001E7BC.s")
/* PROMOTED 2026-10-11 — func_8001E864
 * Source:   cloud/work/frontier/w17g/sources/func_8001E864.c (in-repo, locked)
 * Flags:    -g0 -O2 -mips2 -G 0 -non_shared
 * Evidence: lock:cloud/work/frontier/w17g/sources/func_8001E864.c:func_8001E864 (score0); rom_tu.h context: cloud/work/frontier/w17k/ctx71_O2.jsonl
 * Gate:     full-ROM SHA-1 (promotion transaction)
 */
void *func_8001E864(const void *key, const void *base, int count, int size,
                  int (*compare)(const void *, const void *))
{
    int low;
    int high;
    int middle;
    int result;
    void *element;
    if (count != 0) {
        low = 1;
        high = count;
        do {
            middle = (low + high) >> 1;
            element = (void *)((const unsigned char *)base + size * (middle - 1));
            result = compare(key, element);
            if (result == 0) {
                return element;
            }
            if (result < 0) {
                high = middle - 1;
            } else {
                low = middle + 1;
            }
        } while (high >= low);
    }
    return 0;
}

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

