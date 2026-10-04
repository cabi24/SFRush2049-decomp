/* GENERATED ROM-aligned TU — segment 0x113e0 (rom/lib_113e0)
 * layout map 92c6f1e8103743941a7b4aa34ac1e2aea46d482f3364609a23db9f010ec25ce4; regenerate via `pipeline.layout convert`.
 * Slots are GLOBAL_ASM passthroughs until promoted; do not hand-edit
 * passthrough lines. */
#include "rom_tu.h"

/* PROMOTED 2026-10-04 — func_800107E0
 * Source:   cloud/matches/boot_tail/func_800107E0.c (in-repo, locked)
 * Flags:    -g0 -O2 -mips2 -G 0 -non_shared
 * Evidence: lock:cloud/matches/boot_tail/func_800107E0.c:func_800107E0 (score0); rom_tu.h context: cloud/work/boot_tail_promotion/context.jsonl
 * Gate:     full-ROM SHA-1 (promotion transaction)
 */
extern unsigned char D_8002C630;
extern volatile unsigned char D_8004BE94;
extern void func_80014E10(void);
extern void func_80017108(void);
extern void func_800199F4(void);
extern void func_8001C1D8(unsigned int);
extern void func_8001C390(void);
extern void func_8001E0C0(void);
int func_800107E0(unsigned int frequency)
{
    func_80014E10();
    func_80017108();
    func_800199F4();
    D_8004BE94 = 0;
    func_8001C1D8(frequency);
    func_8001C390();
    func_8001E0C0();
    D_8002C630 = 1;
    return 0;
}

#pragma GLOBAL_ASM("asm/us/nonmatchings/rom/lib_113e0/func_80010840.s")
#pragma GLOBAL_ASM("asm/us/nonmatchings/rom/lib_113e0/func_800108E0.s")
/* PROMOTED 2026-10-04 — func_80010980
 * Source:   cloud/matches/boot_tail/func_80010980.c (in-repo, locked)
 * Flags:    -g0 -O2 -mips2 -G 0 -non_shared
 * Evidence: lock:cloud/matches/boot_tail/func_80010980.c:func_80010980 (score0); rom_tu.h context: cloud/work/boot_tail_promotion/context.jsonl
 * Gate:     full-ROM SHA-1 (promotion transaction)
 */
extern void func_80014488(void);
extern void func_8001E0D4(void);
void func_80010980(void)
{
    if (D_8002C630 != 0) {
        func_80014488();
        func_8001E0D4();
        D_8002C630 = 0;
    }
}

/* PROMOTED 2026-10-04 — func_800109C0
 * Source:   cloud/matches/boot_tail/func_800109C0.c (in-repo, locked)
 * Flags:    -g0 -O2 -mips2 -G 0 -non_shared
 * Evidence: lock:cloud/matches/boot_tail/func_800109C0.c:func_800109C0 (score0); rom_tu.h context: cloud/work/boot_tail_promotion/context.jsonl
 * Gate:     full-ROM SHA-1 (promotion transaction)
 */
extern void func_800144F0(void);
void func_800109C0(void)
{
    if (D_8002C630 != 0) {
        func_800144F0();
        func_8001E0D4();
        D_8002C630 = 0;
    }
}

/* PROMOTED 2026-10-04 — func_80010A00
 * Source:   cloud/matches/boot_tail/func_80010A00.c (in-repo, locked)
 * Flags:    -g0 -O2 -mips2 -G 0 -non_shared
 * Evidence: lock:cloud/matches/boot_tail/func_80010A00.c:func_80010A00 (score0); rom_tu.h context: cloud/work/boot_tail_promotion/context.jsonl
 * Gate:     full-ROM SHA-1 (promotion transaction)
 */
extern u8 D_8002C630;
u8 func_80010A00(void)
{
    return D_8002C630;
}

/* PROMOTED 2026-10-04 — func_80010A0C
 * Source:   cloud/matches/boot_tail/func_80010A0C.c (in-repo, locked)
 * Flags:    -g0 -O2 -mips2 -G 0 -non_shared
 * Evidence: lock:cloud/matches/boot_tail/func_80010A0C.c:func_80010A0C (score0); rom_tu.h context: cloud/work/boot_tail_promotion/context.jsonl
 * Gate:     full-ROM SHA-1 (promotion transaction)
 */
void func_80010A0C(unsigned int value)
{
}

/* PROMOTED 2026-10-04 — func_80010A14
 * Source:   cloud/matches/boot_tail/func_80010A14.c (in-repo, locked)
 * Flags:    -g0 -O2 -mips2 -G 0 -non_shared
 * Evidence: lock:cloud/matches/boot_tail/func_80010A14.c:func_80010A14 (score0); rom_tu.h context: cloud/work/boot_tail_promotion/context.jsonl
 * Gate:     full-ROM SHA-1 (promotion transaction)
 */
extern unsigned char D_8004F810[];
void *func_80010A14(void)
{
    if (D_8002C630 != 0) {
        return D_8004F810;
    }
    return 0;
}

