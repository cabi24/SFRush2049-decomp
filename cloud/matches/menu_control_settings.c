/* flags: -g0 -O3 -mips2 -G 0 -non_shared */
/*
 * Historical label menu_control_settings is misleading: this is the N64
 * version of arcade initroad() (reference/repos/rushtherock/game/road.c):
 * build the car's unit vectors from the start orientation, place the car at
 * start position + body offset, and initialise the four tire and body-corner
 * world positions and road state.
 *
 * N64 differences from the arcade body: three copies of the start uvs
 * (math_utility is the 3x3 copy, i.e. fmatcopy) instead of fmatcopy+makesuvs,
 * an optional call (menu_video_settings) gated by D_801427A1, the position
 * is also written to the per-car record D_80152818[slot], tire rest positions
 * come from the model record at m->car + 0x70 (no `veccopy(..., temp)`), the
 * road code written is 8 and it is copied to a 32-bit per-tire word.
 * func_8009E820(in, out, uvs) is bodtorw.
 *
 * No compile-shaping quirks.  What mattered: vectors must be F32[3] arrays (as
 * in the arcade) and the vecadd operand orders the arcade ones:
 * `vecadd(initin.pos, temp, RWR)` and
 * `vecadd(TIRERWR[i], RWR, TIRERWR[i])`.  With a struct {x,y,z} position, or
 * with the operands commuted, the six loop adds load their operands in the
 * other order (12/164, the residual of cloud/work/ipa-groups/codex_road_context_a140).
 */
typedef signed char s8;
typedef unsigned char u8;
typedef signed short s16;
typedef unsigned short u16;
typedef int s32;
typedef unsigned int u32;
typedef float f32;

#define vecadd(a, b, r) { r[0] = a[0] + b[0]; r[1] = a[1] + b[1]; r[2] = a[2] + b[2]; }
#define veccopy(a, r) { r[0] = a[0]; r[1] = a[1]; r[2] = a[2]; }

typedef struct {
    /* 0x000 */ u32 unk0;
    /* 0x004 */ u32 unk4;
    /* 0x008 */ f32 RWR[3];
    /* 0x014 */ u8 pad14[0x18];
    /* 0x02C */ f32 uvs[3][3];
    /* 0x050 */ u8 pad50[0x20];
    /* 0x070 */ f32 TIRER[4][3];
    /* 0x0A0 */ u8 padA0[0x318];
} CarRec; /* 0x3B8 */

typedef struct {
    /* 0x000 */ CarRec *car;
    /* 0x004 */ u8 pad4[0xF0];
    /* 0x0F4 */ f32 BODYR[4][3];
    /* 0x124 */ u8 pad124[0x108];
    /* 0x22C */ f32 RWR[3];
    /* 0x238 */ u8 pad238[0xC];
    /* 0x244 */ f32 TIRERWR[4][3];
    /* 0x274 */ f32 BODYRWR[4][3];
    /* 0x2A4 */ f32 lastBODYRWR[4][3];
    /* 0x2D4 */ u8 pad2D4[0x18];
    /* 0x2EC */ f32 uvs[3][3];
    /* 0x310 */ u8 pad310[0x2BC];
    /* 0x5CC */ f32 suscomp[4];
    /* 0x5DC */ f32 tpcomp[4];
    /* 0x5EC */ u8 pad5EC[0x20];
    /* 0x60C */ u32 sviscode[4];
    /* 0x61C */ u16 roadcode[4];
    /* 0x624 */ u16 roadboost[4];
    /* 0x62C */ u16 sound_flags[4];
    /* 0x634 */ u8 pad634[0xA0];
    /* 0x6D4 */ f32 initpos[3];
    /* 0x6E0 */ f32 inituvs[3][3];
    /* 0x704 */ f32 initoffset[3];
    /* 0x710 */ u8 pad710[0x3C];
    /* 0x74C */ f32 base_RWR[3];
    /* 0x758 */ u8 pad758[0x3C];
    /* 0x794 */ f32 EPRWR[3];
    /* 0x7A0 */ f32 uvs2[3][3];
    /* 0x7C4 */ u16 unk7C4;
    /* 0x7C6 */ s16 slot;
    /* 0x7C8 */ u8 pad7C8[0x40];
} MODELDAT; /* 0x808 */

extern s8 D_801427A1;
extern CarRec D_80152818[];

void math_utility(f32 (*src)[3], f32 (*dst)[3]);
void func_8009E820(f32 *in, f32 *out, f32 (*uvs)[3]);
void menu_video_settings(MODELDAT *m);

void menu_control_settings(MODELDAT *m) {
    f32 temp[3];
    s32 i;

    math_utility(m->inituvs, m->uvs);
    if (D_801427A1) {
        menu_video_settings(m);
    }
    math_utility(m->inituvs, m->uvs2);
    math_utility(m->inituvs, D_80152818[m->slot].uvs);
    func_8009E820(m->initoffset, temp, m->uvs);
    vecadd(m->initpos, temp, m->RWR);
    veccopy(m->RWR, m->EPRWR);
    veccopy(m->RWR, m->base_RWR);
    veccopy(m->RWR, D_80152818[m->slot].RWR);
    for (i = 0; i < 4; i++) {
        func_8009E820(m->car->TIRER[i], m->TIRERWR[i], m->uvs);
        vecadd(m->TIRERWR[i], m->RWR, m->TIRERWR[i]);
        m->suscomp[i] = 0;
        m->tpcomp[i] = 0;
        m->roadcode[i] = 8;
        m->sviscode[i] = m->roadcode[i];
        m->roadboost[i] = 0;
        m->sound_flags[i] = 0;
        func_8009E820(m->BODYR[i], m->BODYRWR[i], m->uvs);
        vecadd(m->BODYRWR[i], m->RWR, m->BODYRWR[i]);
        veccopy(m->BODYRWR[i], m->lastBODYRWR[i]);
    }
}
