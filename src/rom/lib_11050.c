/* GENERATED ROM-aligned TU — segment 0x11050 (rom/lib_11050)
 * layout map d82332505db016bb10f80b0817ff7cfcf886bd3671d9c2ae19aa7abdf6a87e79; regenerate via `pipeline.layout convert`.
 * Slots are GLOBAL_ASM passthroughs until promoted; do not hand-edit
 * passthrough lines. */
#include "rom_tu.h"

#pragma GLOBAL_ASM("asm/us/nonmatchings/rom/lib_11050/func_80010450.s")
/* PROMOTED 2026-10-04 — func_800105C4
 * Source:   cloud/matches/boot_tail/func_800105C4.c (in-repo, locked)
 * Flags:    -g0 -O2 -mips2 -G 0 -non_shared
 * Evidence: lock:cloud/matches/boot_tail/func_800105C4.c:func_800105C4 (score0); rom_tu.h context: cloud/work/boot_tail_promotion/context.jsonl
 * Gate:     full-ROM SHA-1 (promotion transaction)
 */
extern void osCreateMesgQueue(OSMesgQueue *, OSMesg *, int);
extern unsigned char D_80037FE0[];
extern OSMesg D_80037FF8[];
extern volatile unsigned char D_80037FA0;
extern void (*D_80038020)(void);
extern void func_80010450(void);
void func_800105C4(void)
{
    if (D_80038020 == 0) {
        osCreateMesgQueue((OSMesgQueue *)D_80037FE0, D_80037FF8, 1);
        D_80037FA0 = 0;
        D_80038020 = func_80010450;
    }
}

/* PROMOTED 2026-10-04 — func_8001061C
 * Source:   cloud/matches/boot_tail/func_8001061C.c (in-repo, locked)
 * Flags:    -g0 -O2 -mips2 -G 0 -non_shared
 * Evidence: lock:cloud/matches/boot_tail/func_8001061C.c:func_8001061C (score0); rom_tu.h context: cloud/work/boot_tail_promotion/context.jsonl
 * Gate:     full-ROM SHA-1 (promotion transaction)
 */
void func_8001061C(void)
{
    D_80038020 = 0;
}

/* PROMOTED 2026-10-04 — func_80010628
 * Source:   cloud/work/boot_tail_promotion/sources/func_80010628.c (in-repo, locked)
 * Flags:    -g0 -O2 -mips2 -G 0 -non_shared
 * Evidence: lock:cloud/work/boot_tail_promotion/sources/func_80010628.c:func_80010628 (score0); rom_tu.h context: cloud/work/boot_tail_promotion/context.jsonl
 * Gate:     full-ROM SHA-1 (promotion transaction)
 */
typedef struct TransferRequest { void *destination; void *source; unsigned int size; } TransferRequest;
extern TransferRequest D_80037FA8[];
extern void func_80014594(void);
extern void func_800145DC(void);
extern void func_800105C4(void);
extern void osYieldThread(void);
extern void osInvalDCache(void *, int);
extern int osRecvMesg(OSMesgQueue *, void **, int);
void func_80010628(void *destination, void *source, unsigned int size)
{
    func_80014594();
    func_800105C4();
    if (D_80037FA0 < 4) {
        D_80037FA8[D_80037FA0].destination = destination;
        D_80037FA8[D_80037FA0].source = source;
        D_80037FA8[D_80037FA0].size = (size + 15) & ~15U;
        D_80037FA0++;
        osInvalDCache(destination, size);
        func_800145DC();
        osRecvMesg((OSMesgQueue *) D_80037FE0, 0, 1);
        osYieldThread();
    } else {
        func_800145DC();
    }
}

/* PROMOTED 2026-10-04 — func_80010714
 * Source:   cloud/matches/boot_tail/func_80010714.c (in-repo, locked)
 * Flags:    -g0 -O2 -mips2 -G 0 -non_shared
 * Evidence: lock:cloud/matches/boot_tail/func_80010714.c:func_80010714 (score0); rom_tu.h context: cloud/work/boot_tail_promotion/context.jsonl
 * Gate:     full-ROM SHA-1 (promotion transaction)
 */
extern void func_80010110(void);
void func_80010714(void *destination, void *source, unsigned int size)
{
    func_80014594();
    func_800105C4();
    if (D_80037FA0 < 4) {
        D_80037FA8[D_80037FA0].destination = destination;
        D_80037FA8[D_80037FA0].source = source;
        D_80037FA8[D_80037FA0].size = (size + 15) & ~15U;
        D_80037FA0++;
        osInvalDCache(destination, size);
    }
    func_800145DC();
}

