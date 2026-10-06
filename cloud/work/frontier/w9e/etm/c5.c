/*
 * drone_ai_update (0x80093B20, 820 words) and its only callee entity_tick_main
 * (0x800930A4, 671 words, receives the player index in $s5). By content this is
 * a per-car light/palette/model-slot updater (the "DoDrones" name is a guess).
 *
 * HAND-WRITTEN FIRST PASS (cloud round 5, agent r5_e) from the assembly and the
 * m2c seeds in cloud/work/bigfish/seeds/. Not a match; see STATUS.md.
 *
 * Every other callee is ABI and lives outside this group:
 *   model_data_load, model_transform_setup, func_8008B32C, func_80092FE0,
 *   string_copy_format, func_80092B80, matrix_scale_apply, math_utility.
 */
typedef signed char s8;
typedef unsigned char u8;
typedef signed short s16;
typedef unsigned short u16;
typedef signed int s32;
typedef unsigned int u32;
typedef float f32;

float sqrtf(float);
float fabsf(float);
#pragma intrinsic (sqrtf)
#pragma intrinsic (fabsf)

typedef union {
    u8 b[4];
    u32 w;
} Color;

/* 3x3 matrix followed by a translation; math_utility copies m[9] */
typedef struct Node {
    /* 0x00 */ f32 m[9];
    /* 0x24 */ f32 t[3];
} Node;

/* table at 0x8012E700, 0x44 bytes each; flags bit 31 = hidden */
typedef struct ModelSlot {
    /* 0x00 */ u32 flags;
    /* 0x04 */ u8 pad4[4];
    /* 0x08 */ Node *node;
    /* 0x0C */ u8 padC[8];
    /* 0x14 */ u16 tex;
    /* 0x16 */ s16 child;
    /* 0x18 */ s16 sibling;
    /* 0x1A */ u8 pad1A[0x22];
    /* 0x3C */ u32 colA;
    /* 0x40 */ u32 colB;
} ModelSlot;

/* table at 0x80139320, 0x40 bytes each: four model-slot ids per car */
typedef struct CarModels {
    /* 0x00 */ s32 id[4];
    /* 0x10 */ s32 wheelA;
    /* 0x14 */ s32 wheelB;
    /* 0x18 */ s32 pad18[10];
} CarModels;

/* player_array element (0x3B8 bytes) */
typedef struct Car {
    /* 0x000 */ u8 pad0[0xE8];
    /* 0x0E8 */ u32 flagsE8;
    /* 0x0EC */ u8 padEC[0xF8 - 0xEC];
    /* 0x0F8 */ s16 sF8;
    /* 0x0FA */ u8 padFA[0x344 - 0xFA];
    /* 0x344 */ u16 pal[5];       /* 0x344, 0x346, 0x348, 0x34A: 5:x:x palette indices */
    /* 0x34E */ u8 pad34E[2];
    /* 0x350 */ u32 col350;
    /* 0x354 */ u32 col354;
    /* 0x358 */ u8 pad358[2];
    /* 0x35A */ s8 fade;
    /* 0x35B */ u8 pad35B;
    /* 0x35C */ s8 lightIdx;
    /* 0x35D */ s8 lightMode;
    /* 0x35E */ u8 pad35E;
    /* 0x35F */ s8 nameSet;
    /* 0x360 */ u32 alpha;
    /* 0x364 */ u8 pad364[0x374 - 0x364];
    /* 0x374 */ f32 scale[3];
    /* 0x380 */ u8 pad380;
    /* 0x381 */ u8 b381[3];
    /* 0x384 */ s8 b384;
    /* 0x385 */ u8 pad385[0x38C - 0x385];
    /* 0x38C */ u32 f38C;
    /* 0x390 */ u8 pad390[0x3A1 - 0x390];
    /* 0x3A1 */ u8 b3A1;
    /* 0x3A2 */ u8 pad3A2[0x3B8 - 0x3A2];
} Car;

/* D_8014A250 element (0x808 bytes) */
typedef struct CarState {
    /* 0x000 */ u8 pad0[8];
    /* 0x008 */ u8 mode;
    /* 0x009 */ u8 pad9[0x3D0 - 9];
    /* 0x3D0 */ f32 f3D0;
    /* 0x3D4 */ u8 pad3D4[0x5F4 - 0x3D4];
    /* 0x5F4 */ f32 dist;
    /* 0x5F8 */ u8 pad5F8[0x6C4 - 0x5F8];
    /* 0x6C4 */ s16 s6C4;
    /* 0x6C6 */ u8 pad6C6[0x7D0 - 0x6C6];
    /* 0x7D0 */ s16 s7D0;
    /* 0x7D2 */ u8 pad7D2[0x808 - 0x7D2];
} CarState;

/* the entity drone_ai_update is called with */
typedef struct Entity {
    /* 0x00 */ u8 pad0[6];
    /* 0x06 */ s16 slot;
    /* 0x08 */ s16 car;
    /* 0x0A */ u8 padA[0x14 - 0xA];
    /* 0x14 */ s32 field14;
} Entity;

extern ModelSlot D_8012E700[];
extern CarModels D_80139320[];
extern Car D_80152818[];
extern CarState D_8014A250[];
extern s32 D_8014A110;          /* gameplay_mode */
extern s32 D_8011735C;          /* rng state */
extern Color D_8011B464;
extern s32 D_801174B4;
extern s8 D_80140418;
extern s8 D_8013FECD;
extern s8 D_8013F1D8;
extern u8 D_80140BDC;
extern u16 D_80142998;
extern s8 D_8014978C;
extern s16 D_80151AD0;
extern u16 D_801427C0[];
extern u8 D_801234A4[];
extern u8 *D_8011AF90[];        /* palette base per track */
extern u8 D_8011B468[];         /* stride 4 */
extern s32 D_8011ADC0[][2];
extern f32 D_80123A00, D_80123A04, D_80123A08, D_80123A0C, D_80123A10, D_80123A14;
extern f32 D_80123A18, D_80123A1C, D_80123A20, D_80123A24, D_80123A28, D_80123A2C;
extern f32 D_80123A30, D_80123A34, D_80123A38, D_80123A3C, D_80123A40, D_80123A44;
extern f32 D_80123A48, D_80123A4C, D_80123A50, D_80123A54, D_80123A58, D_80123A5C;
extern f32 D_80123A60;

void model_data_load();
void model_transform_setup();
void math_utility();
void func_8008B32C(Node *, Node *, f32);

s32 func_8009309C(void)
{
    s32 r;
    D_8011735C = D_8011735C * 0x41C64E6D + 12345;
    r = (D_8011735C >> 16) & 0x7FFF;
    return r;
}

s32 func_80093000(void);

void entity_tick_main(s16 idx, s32 hidden)
{
    Color col;
    f32 v[3];
    Node *nodes[3];
    CarModels *mdl;
    Car *car;
    CarState *st;
    Node *src;
    s32 big;
    f32 lim, step, base, floor_;
    f32 f, g, h;

    col = D_8011B464;
    if (hidden) {
        mdl = &D_80139320[idx];
        model_data_load(mdl->id[1], 1, 15);
        model_data_load(mdl->id[2], 1, 15);
        model_data_load(mdl->id[3], 1, 15);
        return;
    }
    mdl = &D_80139320[idx];
    model_transform_setup(mdl->id[1], 0, 15);
    model_transform_setup(mdl->id[2], 0, 15);
    model_transform_setup(mdl->id[3], 0, 15);
    if (D_8014A110 == 2 && idx > 0) {
        col.b[3] = 0x80;
    } else if (D_8014A110 == 6 && (D_80152818[idx].f38C & 1)) {
        col.b[3] = D_80152818[idx].b3A1;
    }
    D_8012E700[(s16)mdl->id[1]].colA = col.w;
    D_8012E700[(s16)mdl->id[2]].colA = col.w;
    D_8012E700[(s16)mdl->id[3]].colA = col.w;

    car = &D_80152818[idx];
    src = D_8012E700[(s16)mdl->id[0]].node;
    v[0] = src->m[3] + src->m[3] + src->t[0];
    v[1] = src->m[4] + src->m[4] + src->t[1];
    v[2] = src->m[5] + src->m[5] + src->t[2];
    v[0] = src->m[6] * -5.6f + v[0];
    v[1] = src->m[7] * -5.6f + v[1];
    v[2] = src->m[8] * -5.6f + v[2];
    nodes[0] = D_8012E700[(s16)mdl->id[1]].node;
    math_utility(src, nodes[0]);
    nodes[0]->t[0] = v[0];
    nodes[0]->t[1] = v[1];
    nodes[0]->t[2] = v[2];
    nodes[1] = D_8012E700[(s16)mdl->id[2]].node;
    math_utility(src, nodes[1]);
    nodes[1]->t[0] = v[0];
    nodes[1]->t[1] = v[1];
    nodes[1]->t[2] = v[2];
    nodes[2] = D_8012E700[(s16)mdl->id[3]].node;
    math_utility(src, nodes[2]);
    nodes[2]->t[0] = v[0];
    nodes[2]->t[1] = v[1];
    nodes[2]->t[2] = v[2];

    big = 0;
    if (D_8014A110 == 6 && car->b384 == 3) {
        big = 1;
        car->scale[0] = 0.1f;
        car->scale[1] = 0.1f;
        car->scale[2] = 0.2f;
    } else if ((car->sF8 >> 2) < 0) {
        car->scale[0] = 0.25f - (f32)func_8009309C() * 0.15f / 32768.0f;
        car->scale[1] = 0.25f - (f32)func_8009309C() * 0.15f / 32768.0f;
        car->scale[2] = 0.55f - (f32)func_8009309C() * 0.1f / 32768.0f;
    } else {
        st = &D_8014A250[idx];
        if (st->f3D0 < 0.1f) {
            if (car->scale[0] < 0.28f) {
                car->scale[0] = 0.25f - (f32)func_8009309C() * 0.15f / 32768.0f;
            } else {
                car->scale[0] -= (f32)func_8009309C() * 0.025f / 32768.0f + 0.01f;
                if (car->scale[0] < 0.2f) car->scale[0] = 0.2f;
            }
            if (car->scale[1] < 0.28f) {
                car->scale[1] = 0.25f - (f32)func_8009309C() * 0.15f / 32768.0f;
            } else {
                car->scale[1] -= (f32)func_8009309C() * 0.025f / 32768.0f + 0.01f;
                if (car->scale[1] < 0.2f) car->scale[1] = 0.2f;
            }
            if (car->scale[2] < 0.58f) {
                car->scale[2] = 0.55f - (f32)func_8009309C() * 0.1f / 32768.0f;
            } else {
                car->scale[2] -= (f32)func_8009309C() * 0.025f / 32768.0f + 0.01f;
                if (car->scale[2] < 0.5f) car->scale[2] = 0.5f;
            }
        } else {
            f = (f32)st->s7D0 * 1.35f / 10000.0f + 0.2f;
            if (f > 1.0f) f = 1.0f;
            car->scale[0] = f - (f32)func_8009309C() * 0.15f / 32768.0f;
            car->scale[1] = f - (f32)func_8009309C() * 0.15f / 32768.0f;
            f = (f32)st->s7D0 / 10000.0f + 0.5f;
            if (f > 1.0f) f = 1.0f;
            car->scale[2] = f - (f32)func_8009309C() * 0.1f / 32768.0f;
        }
    }
    nodes[0]->m[6] *= car->scale[0];
    nodes[0]->m[7] *= car->scale[0];
    nodes[0]->m[8] *= car->scale[0];
    nodes[1]->m[6] *= car->scale[1];
    nodes[1]->m[7] *= car->scale[1];
    nodes[1]->m[8] *= car->scale[1];
    if (big) {
        func_8008B32C(nodes[2], nodes[2], car->scale[2]);
    } else {
        nodes[2]->m[0] *= car->scale[2];
        nodes[2]->m[1] *= car->scale[2];
        nodes[2]->m[2] *= car->scale[2];
        nodes[2]->m[3] *= car->scale[2];
        nodes[2]->m[4] *= car->scale[2];
        nodes[2]->m[5] *= car->scale[2];
    }
    if (D_8014A110 == 6 && D_8014A250[idx].s6C4 >= 0) {
        D_8012E700[mdl->id[1]].flags |= 0x80000000;
        D_8012E700[mdl->id[2]].flags |= 0x80000000;
        D_8012E700[mdl->id[3]].flags |= 0x80000000;
    } else if (D_80140418 != 0 || (car->flagsE8 & 8)) {
        D_8012E700[mdl->id[1]].flags ^= 0x80000000;
        D_8012E700[mdl->id[2]].flags ^= 0x80000000;
        D_8012E700[mdl->id[3]].flags ^= 0x80000000;
    } else {
        D_8012E700[mdl->id[1]].flags &= 0x7FFFFFFF;
        D_8012E700[mdl->id[2]].flags &= 0x7FFFFFFF;
        D_8012E700[mdl->id[3]].flags &= 0x7FFFFFFF;
    }
}

/* STAND-IN caller (not the real drone_ai_update body) */
void drone_ai_update(Entity *ent, s16 alive)
{
    s32 carIdx;
    Car *car;
    carIdx = ent->car;
    car = &D_80152818[carIdx];
    if (alive == 0) { ent->slot = -1; return; }
    if (D_8014A250[carIdx].mode == 2) {
        entity_tick_main(carIdx, (car->flagsE8 & 0x10) != 0);
    }
    model_data_load(ent->slot, 1, 15);
    if (D_8014A250[carIdx].mode == 3) entity_tick_main(carIdx, ent->field14);
    car->col354 = car->col350;
}
