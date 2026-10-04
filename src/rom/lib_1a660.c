/* GENERATED ROM-aligned TU — segment 0x1a660 (rom/lib_1a660)
 * layout map 91b72acb516b2291b9dbf568e33b80bbc7fb9c8413206c2c73c661eb5c01ac02; regenerate via `pipeline.layout convert`.
 * Slots are GLOBAL_ASM passthroughs until promoted; do not hand-edit
 * passthrough lines. */
#include "rom_tu.h"

#pragma GLOBAL_ASM("asm/us/nonmatchings/rom/lib_1a660/func_80019A60.s")
/* PROMOTED 2026-10-04 — func_80019AA8
 * Source:   cloud/matches/boot_tail/func_80019AA8.c (in-repo, locked)
 * Flags:    -g0 -O2 -mips2 -G 0 -non_shared
 * Evidence: lock:cloud/matches/boot_tail/func_80019AA8.c:func_80019AA8 (score0); rom_tu.h context: cloud/work/boot_tail_promotion/context.jsonl
 * Gate:     full-ROM SHA-1 (promotion transaction)
 */
extern unsigned int D_8004FA20[];
unsigned int func_80019AA8(unsigned char *state)
{
    return D_8004FA20[state[75] == 255 ? 8 : state[75]];
}

#pragma GLOBAL_ASM("asm/us/nonmatchings/rom/lib_1a660/func_80019AD4.s")
/* PROMOTED 2026-10-04 — func_80019BE4
 * Source:   cloud/matches/boot_tail/func_80019BE4.c (in-repo, locked)
 * Flags:    -g0 -O2 -mips2 -G 0 -non_shared
 * Evidence: lock:cloud/matches/boot_tail/func_80019BE4.c:func_80019BE4 (score0); rom_tu.h context: cloud/work/boot_tail_promotion/context.jsonl
 * Gate:     full-ROM SHA-1 (promotion transaction)
 */
#pragma pack(1)
typedef struct VoiceState { u8 unknown00[36]; u32 flags; u8 unknown28[34]; u8 channel; u8 set; u8 unknown4C[64]; u32 current8C; u32 stored90; u32 fixed94; u8 mode98; u8 unknown99[40]; u8 lastC1; } VoiceState;
#pragma pack(0)
extern void func_80020FDC(u8, u8, u8);
void func_80019BE4(VoiceState *state)
{
    if (!(state->flags & 0x80000)) {
        if (state->mode98 == 1) {
            if (!(state->flags & 0x2000)) state->current8C = 0;
            else state->current8C = state->stored90;
        } else {
            state->current8C = state->stored90;
        }
        state->fixed94 = (u32)state->lastC1 << 16;
    }
    if (state->channel != 255) func_80020FDC(state->channel, state->set, 1);
}

/* PROMOTED 2026-10-04 — func_80019C8C
 * Source:   cloud/work/boot_tail_promotion/sources/func_80019C8C.c (in-repo, locked)
 * Flags:    -g0 -O2 -mips2 -G 0 -non_shared
 * Evidence: lock:cloud/work/boot_tail_promotion/sources/func_80019C8C.c:func_80019C8C (score0); rom_tu.h context: cloud/work/boot_tail_promotion/context.jsonl
 * Gate:     full-ROM SHA-1 (promotion transaction)
 */
#pragma pack(1)
typedef struct VoiceState_80019C8C { u8 unknown00[16];u32 child,parent;u8 unknown18[12]; u32 flags;u8 unknown28[34];u8 channel,set;u8 unknown4C[2]; u16 base_key,key;u8 unknown52[14];u32 identifier; u8 unknown64[40];u32 glide;u8 unknown90[4];u32 pitch; u8 unknown98[40];s8 cents;u8 original_key;u8 unknownC2[222]; } VoiceState_80019C8C;
#pragma pack(0)
extern VoiceState_80019C8C D_8004BEB8[];
extern u8 D_8004FA18;
extern u8 func_8001467C(u32);
extern void func_8001EB10(VoiceState_80019C8C *);
extern u32 func_8001ECE0(VoiceState_80019C8C *);
extern void func_800217E4(VoiceState_80019C8C *);
extern void func_80020F4C(u8,u8,u8);
u32 func_80019C8C(u8 key,u8 channel,u8 set)
{
    u32 i,result,previous;
    VoiceState_80019C8C *state,*last;
    result=0xFFFFFFFFU;
    for (i=0,state=D_8004BEB8;i<D_8004FA18;i++,state++) {
        if (state->identifier!=0xFFFFFFFFU && state->channel==channel && state->set==set &&
            (state->flags&16) && (!(state->flags&8) || (state->flags&0x40000000)) &&
            func_8001467C(i)) {
            last=state;
            state->pitch=((u32)state->key<<16)+(state->cents*65536)/100;
            state->original_key=state->key;
            state->key=key+(state->key&255)-(u8)state->base_key;
            state->base_key=key;
            state->cents=0;
            state->glide=0;
            state->flags|=0x80800;
            func_8001EB10(&D_8004BEB8[i]);
            if (result==0xFFFFFFFFU) {
                state->child=0xFFFFFFFFU;
                state->parent=0xFFFFFFFFU;
                result=func_8001ECE0(&D_8004BEB8[i]);
                previous=state->identifier;
            } else {
                D_8004BEB8[(previous&255)].child=state->identifier;
                state->parent=previous;
                previous=state->identifier;
            }
        }
    }
    if (result!=0xFFFFFFFFU) {
        func_800217E4(last);
        func_80020F4C(last->channel,last->set,(u8)last->key);
    }
    return result;
}

#pragma GLOBAL_ASM("asm/us/nonmatchings/rom/lib_1a660/func_80019ED0.s")
#pragma GLOBAL_ASM("asm/us/nonmatchings/rom/lib_1a660/func_80019F48.s")
#pragma GLOBAL_ASM("asm/us/nonmatchings/rom/lib_1a660/func_8001A270.s")
#pragma GLOBAL_ASM("asm/us/nonmatchings/rom/lib_1a660/func_8001A5D8.s")
#pragma GLOBAL_ASM("asm/us/nonmatchings/rom/lib_1a660/func_8001A658.s")
/* PROMOTED 2026-10-04 — func_8001B154
 * Source:   cloud/matches/boot_tail/func_8001B154.c (in-repo, locked)
 * Flags:    -g0 -O2 -mips2 -G 0 -non_shared
 * Evidence: lock:cloud/matches/boot_tail/func_8001B154.c:func_8001B154 (score0); rom_tu.h context: cloud/work/boot_tail_promotion/context.jsonl
 * Gate:     full-ROM SHA-1 (promotion transaction)
 */
extern s32 D_8004F808;
extern s32 D_8004F800;
extern u32 D_8004BE90;
extern void func_8001A658(void);
void func_8001B154(void)
{
    if (D_8004F808 != 0) {
        D_8004BE90 = (u32)((s32)((u32)D_8004F808 * 32000U) / D_8004F800) << 3;
        func_8001A658();
    }
}

#pragma GLOBAL_ASM("asm/us/nonmatchings/rom/lib_1a660/func_8001B1D0.s")
#pragma GLOBAL_ASM("asm/us/nonmatchings/rom/lib_1a660/func_8001B29C.s")
#pragma GLOBAL_ASM("asm/us/nonmatchings/rom/lib_1a660/func_8001B3A0.s")
#pragma GLOBAL_ASM("asm/us/nonmatchings/rom/lib_1a660/func_8001B4A4.s")
#pragma GLOBAL_ASM("asm/us/nonmatchings/rom/lib_1a660/func_8001B5F4.s")
/* PROMOTED 2026-10-04 — func_8001B744
 * Source:   cloud/matches/boot_tail/func_8001B744.c (in-repo, locked)
 * Flags:    -g0 -O2 -mips2 -G 0 -non_shared
 * Evidence: lock:cloud/matches/boot_tail/func_8001B744.c:func_8001B744 (score0); rom_tu.h context: cloud/work/boot_tail_promotion/context.jsonl
 * Gate:     full-ROM SHA-1 (promotion transaction)
 */
struct VoiceState;
extern void func_80020DA8(u8, struct VoiceState *, struct VoiceState *);
void func_8001B744(struct VoiceState *destination, struct VoiceState *source)
{
    func_80020DA8(7, destination, source);
    func_80020DA8(10, destination, source);
    func_80020DA8(91, destination, source);
    func_80020DA8(128, destination, source);
    func_80020DA8(132, destination, source);
}

#pragma GLOBAL_ASM("asm/us/nonmatchings/rom/lib_1a660/func_8001B7C0.s")
#pragma GLOBAL_ASM("asm/us/nonmatchings/rom/lib_1a660/func_8001B8C4.s")
#pragma GLOBAL_ASM("asm/us/nonmatchings/rom/lib_1a660/func_8001B968.s")
#pragma GLOBAL_ASM("asm/us/nonmatchings/rom/lib_1a660/func_8001B9F8.s")
#pragma GLOBAL_ASM("asm/us/nonmatchings/rom/lib_1a660/func_8001BDB8.s")
#pragma GLOBAL_ASM("asm/us/nonmatchings/rom/lib_1a660/func_8001BE14.s")
#pragma GLOBAL_ASM("asm/us/nonmatchings/rom/lib_1a660/func_8001C19C.s")
#pragma GLOBAL_ASM("asm/us/nonmatchings/rom/lib_1a660/func_8001C1D8.s")
