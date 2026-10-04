/* GENERATED ROM-aligned TU — segment 0x25bb0 (rom/lib_25bb0)
 * layout map f5baec304e2af6053d0e085c8e4b667c97ff73771969d95aeccac477c5f6c448; regenerate via `pipeline.layout convert`.
 * Slots are GLOBAL_ASM passthroughs until promoted; do not hand-edit
 * passthrough lines. */
#include "rom_tu.h"

/* PROMOTED 2026-10-04 — func_80024FB0
 * Source:   cloud/matches/boot_tail/func_80024FB0.c (in-repo, locked)
 * Flags:    -g0 -O2 -mips2 -G 0 -non_shared
 * Evidence: lock:cloud/matches/boot_tail/func_80024FB0.c:func_80024FB0 (score0); rom_tu.h context: cloud/work/boot_tail_promotion/context.jsonl
 * Gate:     full-ROM SHA-1 (promotion transaction)
 */
typedef struct StreamState { unsigned char unknown_0000[4573]; signed char state; signed char scale; unsigned char unknown_11DF[33]; volatile unsigned char busy; unsigned char unknown_1201[39]; } StreamState;
extern unsigned char D_80038290;
void func_80024FB0(StreamState *stream)
{
    if (D_80038290 != 0) {
        stream->scale *= 2;
    }
}

#pragma GLOBAL_ASM("asm/us/nonmatchings/rom/lib_25bb0/func_80024FD4.s")
/* PROMOTED 2026-10-04 — func_8002506C
 * Source:   cloud/matches/boot_tail/func_8002506C.c (in-repo, locked)
 * Flags:    -g0 -O2 -mips2 -G 0 -non_shared
 * Evidence: lock:cloud/matches/boot_tail/func_8002506C.c:func_8002506C (score0); rom_tu.h context: cloud/work/boot_tail_promotion/context.jsonl
 * Gate:     full-ROM SHA-1 (promotion transaction)
 */
extern void func_80010714(void *, void *, unsigned int);
extern int osJamMesg(OSMesgQueue *, void *, int);
void func_8002506C(void *source, void *destination, unsigned int count,
                   OSMesgQueue *queue)
{
    func_80010714(destination, source, count);
    osJamMesg(queue, 0, 1);
}

/* PROMOTED 2026-10-04 — func_800250AC
 * Source:   cloud/matches/boot_tail/func_800250AC.c (in-repo, locked)
 * Flags:    -g0 -O2 -mips2 -G 0 -non_shared
 * Evidence: lock:cloud/matches/boot_tail/func_800250AC.c:func_800250AC (score0); rom_tu.h context: cloud/work/boot_tail_promotion/context.jsonl
 * Gate:     full-ROM SHA-1 (promotion transaction)
 */
extern void osCreateMesgQueue(OSMesgQueue *, OSMesg *, int);
extern int osJamMesg(OSMesgQueue *, OSMesg, int);
extern OSMesgQueue D_800586A8;
extern OSMesg D_800586C0;
void func_800250AC(void)
{
    osCreateMesgQueue(&D_800586A8, &D_800586C0, 1);
    osJamMesg(&D_800586A8, 0, 0);
}

#pragma GLOBAL_ASM("asm/us/nonmatchings/rom/lib_25bb0/func_800250F0.s")
#pragma GLOBAL_ASM("asm/us/nonmatchings/rom/lib_25bb0/func_80025120.s")
#pragma GLOBAL_ASM("asm/us/nonmatchings/rom/lib_25bb0/func_80025150.s")
#pragma GLOBAL_ASM("asm/us/nonmatchings/rom/lib_25bb0/func_8002517C.s")
#pragma GLOBAL_ASM("asm/us/nonmatchings/rom/lib_25bb0/func_800251A8.s")
/* PROMOTED 2026-10-04 — func_80025264
 * Source:   cloud/work/boot_tail_promotion/sources/func_80025264.c (in-repo, locked)
 * Flags:    -g0 -O2 -mips2 -G 0 -non_shared
 * Evidence: lock:cloud/work/boot_tail_promotion/sources/func_80025264.c:func_80025264 (score0); rom_tu.h context: cloud/work/boot_tail_promotion/context.jsonl
 * Gate:     full-ROM SHA-1 (promotion transaction)
 */
typedef struct StreamState_80025264 { unsigned char unknown_0000[358]; unsigned short block_count; unsigned char unknown_0168[40]; void (*callback)(void *, void *, unsigned int, OSMesgQueue *); void *data; void *argument2; unsigned int argument3; unsigned char unknown_01A0[4096]; unsigned short read_count; unsigned short write_count; unsigned int buffered; int remaining; OSMesgQueue queue; OSMesg message; unsigned int field_11C8; unsigned int field_11CC; unsigned int field_11D0; unsigned int consumed; unsigned int available; signed char field_11DC; signed char state; signed char scale; unsigned char unknown_11DF[33]; volatile unsigned char busy; unsigned char unknown_1201[27]; unsigned int processed; float field_1220; unsigned int token; } StreamState_80025264;
extern StreamState_80025264 D_80056230[2];
int func_80025264(unsigned int token)
{
    int i;

    for (i = 0; i < 2; i++) {
        if (D_80056230[i].busy && D_80056230[i].token == token) {
            return i;
        }
    }
    return -1;
}

#pragma GLOBAL_ASM("asm/us/nonmatchings/rom/lib_25bb0/func_800252AC.s")
#pragma GLOBAL_ASM("asm/us/nonmatchings/rom/lib_25bb0/func_800254D4.s")
/* PROMOTED 2026-10-04 — func_80025594
 * Source:   cloud/matches/boot_tail/func_80025594.c (in-repo, locked)
 * Flags:    -g0 -O2 -mips2 -G 0 -non_shared
 * Evidence: lock:cloud/matches/boot_tail/func_80025594.c:func_80025594 (score0); rom_tu.h context: cloud/work/boot_tail_promotion/context.jsonl
 * Gate:     full-ROM SHA-1 (promotion transaction)
 */
extern volatile unsigned char D_8002D480[];
extern int func_80025264(unsigned int);
extern void func_800254D4(int);
int func_80025594(unsigned int token)
{
    int selected;

    if (token != (unsigned int)-1 && D_8002D480[0]) {
        selected = func_80025264(token);
        if (selected != -1) {
            func_800254D4(selected);
            return 1;
        }
    }
    return 0;
}

#pragma GLOBAL_ASM("asm/us/nonmatchings/rom/lib_25bb0/func_800255F0.s")
#pragma GLOBAL_ASM("asm/us/nonmatchings/rom/lib_25bb0/func_80025670.s")
#pragma GLOBAL_ASM("asm/us/nonmatchings/rom/lib_25bb0/func_8002574C.s")
#pragma GLOBAL_ASM("asm/us/nonmatchings/rom/lib_25bb0/func_800259A8.s")
#pragma GLOBAL_ASM("asm/us/nonmatchings/rom/lib_25bb0/func_80025AB4.s")
#pragma GLOBAL_ASM("asm/us/nonmatchings/rom/lib_25bb0/func_80025C68.s")
#pragma GLOBAL_ASM("asm/us/nonmatchings/rom/lib_25bb0/func_80025D84.s")
#pragma GLOBAL_ASM("asm/us/nonmatchings/rom/lib_25bb0/func_80025DC0.s")
