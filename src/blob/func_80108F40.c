/* flags: -g0 -O3 -mips2 -G 0 -non_shared */
/*
 * func_80108F40: split-screen radar-car Blit AnimFunc, N64 port of arcade game/hud.c AnimateTraffic
 * (Hidden is the arcade hud.c helper; umerge inlines the kept Hidden from state_update_global.c, so it is
 * a static copy here, as in func_80109A60).  AnimID bits 8-11 = viewing player `me`, bits 0-3 = car `slot`.
 * Hidden (and AnimFunc cleared) when `me` is not a player in this race, state_word_a bit 3 is set or
 * D_801543CA < 2; hidden when the radar is off (D_80156CE8) or the viewer's car state (+0xEF) is 1; hidden
 * (AnimFunc cleared) when model[slot] is not in game; hidden when slot != me and !are_we_solid(slot)
 * (func_800CF604).  Otherwise lpos = car[slot].dr_pos - car[me].dr_pos (arcade vecsub, written out),
 * frwtobod (func_800A61B0) into the viewer's frame, then X = radar width/2 +- rpos[0]/480 * width
 * (mirrored when D_80140A04), Y = 16 - rpos[2]/480 * height; clipped to the radar frame (size
 * D_8011617C x D_80116180, published by func_80108DA8) less the car icon size (Width/8, Height); offset by
 * the split-screen row D_80116028[count-1][me]; SelectBlit (stat_race_update) picks the human/drone icon;
 * centred and re-applied with UpdateBlit (Input_ApplyPadConfig).
 * score.py fn: MATCH; whole-program unit: EQUAL, 0 locked bodies differ (w16h).
 *
 * Shaping / disclosures:
 *  - UNUSED LOCALS (exact frame residual): `ldir[3]`, `angle`, `flip` (arcade AnimateTraffic names; the N64
 *    port dropped the arcade direction/angle code).  Retail frame 168: me@164, m/gc/mc/scale@148..160,
 *    slot@146, rpos@132, lpos@120, [104..119], hw@100, hh@96.  Without these three declarations the frame
 *    is 152 and hw/hh sit at 116/112; they take exactly the 16 bytes 104..119 (4 slots: 12 + 2 x 2).
 *  - `scale` is assigned the same literal in both arms: retail has two copies of 1/480 in .rodata
 *    (0x801248CC, 0x801248D0) and reuses the arm's $f0 (negated) for Y.
 *  - the viewer's car-state byte is read through D_80152818 (CAR_DATA +0xEF), not D_801528F0 as in
 *    func_80108DA8: with the separate symbol the car base address becomes a coloured web (t0).
 *  - gc = &car[slot] is assigned before the in_game test (retail gets no .noalias for it, so the slot
 *    car's loads stay behind the lpos stores); `X = ...` precedes `Y = 16`; `+=` for the commutative
 *    operand order of the add.s.
 */
typedef signed char s8; typedef unsigned char u8; typedef signed short s16; typedef unsigned short u16;
typedef signed int s32; typedef unsigned int u32; typedef float f32;

typedef struct Blit {
    /* 0x00 */ char *Name;
    /* 0x04 */ void *Image;
    /* 0x08 */ void *Info;
    /* 0x0C */ s16 TexIndex;
    /* 0x0E */ s16 X;
    /* 0x10 */ s16 Y;
    /* 0x12 */ u16 State;
    /* 0x14 */ s16 Width;
    /* 0x16 */ s16 Height;
    /* 0x18 */ u8 Alpha, Flip;
    /* 0x1A */ s8 Hide;
    /* 0x1B */ u8 Init;
    /* 0x1C */ s16 Top, Bot, Left, Right;
    /* 0x24 */ s32 pad24;
    /* 0x28 */ void *AnimFunc;
    /* 0x2C */ u32 AnimID;
} Blit;

typedef struct MODELDAT {
    u8 pad0[1990];
    s16 index;              /* 1990 */
    s16 in_game;            /* 1992 */
    u8 pad7CA[2];
    s8 drone_type;          /* 1996 */
    u8 pad7CD[2056 - 1997];
} MODELDAT;
typedef struct CAR_DATA {
    u8 pad0[8];
    f32 dr_pos[3];          /* 8 */
    u8 pad14[44 - 20];
    f32 dr_uvs[3][3];       /* 44 */
    u8 pad50[0xEF - 80];
    s8 state;
    u8 padF0[952 - 0xF0];
} CAR_DATA;
typedef struct { s32 x, y; } Pos;

extern s16 gNumWindows;
extern volatile s16 D_801543CA;
extern s8 D_80156CE8;
extern s8 D_80140A04;
extern s32 state_word_a, D_8011617C, D_80116180;
extern Pos D_80116028[][4];
extern MODELDAT D_8014A250[];
extern CAR_DATA D_80152818[];
extern void Input_ApplyPadConfig(void *);
extern s32 func_800CF604(s16);
extern void func_800A61B0(f32 *, f32 *, f32 *);
extern void stat_race_update(Blit *, s32, s32, s32);

static s32 Hidden(Blit *blt, s32 hide)
{
    if (hide != blt->Hide) {
        blt->Hide = hide;
        Input_ApplyPadConfig(blt);
    }
    return blt->Hide;
}

s32 func_80108F40(Blit *blt) {
    s32 me;
    MODELDAT *m;
    CAR_DATA *gc;
    CAR_DATA *mc;
    f32 scale;
    s16 slot;
    f32 rpos[3];
    f32 lpos[3];
    f32 ldir[3];
    s16 angle, flip;
    s32 hw;
    s32 hh;

    me = (blt->AnimID >> 8) & 0xF;
    hw = blt->Width * 0.125f;
    hh = blt->Height;
    if (me >= gNumWindows || (state_word_a & 8) || D_801543CA < 2) {
        blt->AnimFunc = 0;
        return Hidden(blt, 1);
    }
    slot = blt->AnimID & 0xF;
    if (Hidden(blt, !D_80156CE8 || D_80152818[me].state == 1)) {
        return 1;
    }
    m = &D_8014A250[slot];
    gc = &D_80152818[slot];
    if (!m->in_game) {
        blt->AnimFunc = 0;
        return Hidden(blt, 1);
    }
    if (Hidden(blt, slot != me && !func_800CF604(slot))) {
        return 1;
    }
    mc = &D_80152818[me];
    lpos[0] = gc->dr_pos[0] - mc->dr_pos[0];
    lpos[1] = gc->dr_pos[1] - mc->dr_pos[1];
    lpos[2] = gc->dr_pos[2] - mc->dr_pos[2];
    func_800A61B0(lpos, rpos, mc->dr_uvs[0]);
    blt->X = D_8011617C * 0.5f;
    blt->Y = 16;
    if (D_80140A04) {
        scale = (1.0f / 480.0f);
        blt->X += -rpos[0] * scale * D_8011617C;
    } else {
        scale = (1.0f / 480.0f);
        blt->X += rpos[0] * scale * D_8011617C;
    }
    blt->Y += rpos[2] * -scale * D_80116180;
    if (Hidden(blt, blt->X < hw || D_8011617C - hw < blt->X || blt->Y < hh || D_80116180 - hh < blt->Y)) {
        return 1;
    }
    blt->X += D_80116028[gNumWindows - 1][me].x;
    blt->Y += D_80116028[gNumWindows - 1][me].y;
    if (m->drone_type == 2) {
        stat_race_update(blt, m->index, blt->Width * 0.125f, blt->Height);
    } else if (m->drone_type == 1) {
        stat_race_update(blt, 5, blt->Width * 0.125f, blt->Height);
    }
    blt->X -= hw / 2;
    blt->Y -= hh / 2;
    Input_ApplyPadConfig(blt);
    return 1;
}
