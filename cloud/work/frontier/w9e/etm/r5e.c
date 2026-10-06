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
extern s32 D_8011B464;
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
void matrix_scale_apply();
void math_utility();
void func_8008B32C();
void func_80092FE0();
s16 func_80092B80();
u16 string_copy_format();

#define RAND15() ((D_8011735C = D_8011735C * 0x41C64E6D + 12345), ((D_8011735C >> 16) & 0x7FFF))

void entity_tick_main(s16 ipa_s5, s32 flag);

void drone_ai_update(Entity *ent, s16 alive)
{
    Color pal;            /* averaged palette colour */
    Color far;
    Color colA, colB, colC;
    s32 slotTex;
    s32 carIdx;
    s32 isHidden;
    Car *car;
    CarState *st;
    CarModels *mdl;
    s32 modelB, modelA;
    s32 wheelA, wheelB;
    u8 *pbase;
    f32 d, k, nk;
    s32 slot = ent->slot;

    carIdx = ent->car;
    slotTex = D_8012E700[slot].tex;
    if (alive == 0) {
        if (slot >= 0) {
            model_data_load(slot, 1, 15);
        }
        ent->field14 = 0;
        ent->slot = -1;
        return;
    }
    car = &D_80152818[carIdx];
    isHidden = (car->flagsE8 & 0x10) != 0;
    mdl = &D_80139320[carIdx];
    wheelB = mdl->wheelB;
    wheelA = mdl->wheelA;
    st = &D_8014A250[carIdx];
    if (st->mode == 2) {
        entity_tick_main(carIdx, isHidden);
    }

    pbase = D_8011AF90[D_8014978C];
    {
        u8 *p0 = pbase + ((car->pal[0] & 0xF800) >> 11) * 4;
        u8 *p1 = pbase + ((car->pal[1] & 0xF800) >> 11) * 4;
        u8 *p2 = pbase + ((car->pal[2] & 0xF800) >> 11) * 4;
        u8 *p3 = pbase + ((car->pal[3] & 0xF800) >> 11) * 4;
        pal.b[0] = (p3[0] + p0[0] + p1[0] + p2[0]) >> 2;
        pal.b[1] = (p3[1] + p0[1] + p1[1] + p2[1]) >> 2;
        pal.b[2] = (p3[2] + p0[2] + p1[2] + p2[2]) >> 2;
    }
    d = st->dist;
    if (d >= 20.0f) {
        k = (d - 20.0f) * D_80123A60;
        if (k > 1.0f) {
            far.w = *(u32 *)(pbase + D_8011B468[D_8014978C * 4] * 4);
            pal = far;
        } else if (k > 0.0f) {
            nk = 1.0f - k;
            far.w = *(u32 *)(pbase + D_8011B468[D_8014978C * 4] * 4);
            pal.b[0] = (u32) ((f32)(u32) pal.b[0] * nk + (f32)(u32) far.b[0] * k);
            pal.b[1] = (u32) ((f32)(u32) pal.b[1] * nk + (f32)(u32) far.b[1] * k);
            pal.b[2] = (u32) ((f32)(u32) pal.b[2] * nk + (f32)(u32) far.b[2] * k);
            pal.b[3] = 0xFF;
        } else {
            pal.b[3] = 0xFF;
        }
    } else {
        pal.b[3] = 0xFF;
    }
    colA.w = pal.w;
    car->col354 = car->col350;
    car->col350 = colA.w;
    colB.w = D_8011ADC0[D_8014978C][0];
    colC.w = D_8011ADC0[D_8014978C][1];

    if (car->fade != 0) {
        if (car->alpha < 0x20) {
            car->alpha = 0;
        } else {
            car->alpha -= 0x19;
        }
    } else {
        if (car->alpha >= 0xE1) {
            car->alpha = 0xFF;
        } else {
            car->alpha += 0x19;
        }
    }
    colB.b[3] = car->alpha;
    colC.b[3] = car->alpha;
    if (D_8014A110 == 2 && carIdx > 0) {
        colA.b[3] = 0x80;
    } else if (D_8014A110 == 6 && (car->f38C & 1)) {
        colA.b[3] = car->b3A1;
        colB.b[3] = car->b3A1;
        colC.b[3] = car->b3A1;
    } else {
        colA.b[3] = 0xFF;
    }
    func_80092FE0(carIdx, &colA.w);
    D_8012E700[(s16) mdl->wheelB].colA = colB.w;
    D_8012E700[(s16) mdl->wheelB].colB = colC.w;

    if (isHidden) {
        car->nameSet = 0;
        if (func_80092B80(carIdx, 13) != slotTex) {
            D_8012E700[ent->slot].tex = func_80092B80(carIdx, 13);
        }
        model_data_load((s16) wheelA, 1, 15);
        model_data_load((s16) wheelB, 1, 15);
        matrix_scale_apply();
        return;
    }
    if ((D_801174B4 & 0x100) && D_8013FECD != 0 && car->nameSet == 0) {
        D_80142998 = string_copy_format(D_801234A4, 0, D_80140BDC - 1, 0);
        car->nameSet = 1;
    }
    if ((D_801174B4 & 0x100) && D_8013FECD == 0 && car->nameSet != 0) {
        car->nameSet = 0;
    }
    if (car->nameSet != 0) {
        if (slotTex != D_80142998) {
            D_8012E700[ent->slot].tex = D_80142998;
        }
        model_data_load((s16) wheelA, 1, 15);
        model_data_load((s16) wheelB, 1, 15);
        matrix_scale_apply();
        return;
    }
    if (!(D_801174B4 & 0x100)) {
        matrix_scale_apply();
        if (car->fade != 0) {
            if (car->alpha < 0x11) {
                model_data_load((s16) wheelB, 1, 15);
            } else {
                colB.b[3] = car->alpha;
                colC.b[3] = car->alpha;
                D_8012E700[(s16) mdl->wheelB].colA = colB.w;
                D_8012E700[(s16) mdl->wheelB].colB = colC.w;
            }
        } else {
            if ((D_8014A110 == 6 || D_8014A110 == 4) && D_80151AD0 >= 3) {
                model_data_load((s16) wheelB, 1, 15);
            } else if ((D_801174B4 & 0x7C0000) && D_8013F1D8 != 0) {
                model_data_load((s16) wheelB, 1, 15);
            } else if (car->lightIdx < 0 || car->lightMode >= 2) {
                model_transform_setup(wheelB, 0, 15);
            } else {
                model_data_load((s16) wheelB, 1, 1 << car->lightIdx);
            }
        }
        if (func_80092B80(carIdx, st->mode) != slotTex) {
            D_8012E700[ent->slot].tex = func_80092B80(carIdx, st->mode);
        }
        if (D_8014A110 == 6 && st->s6C4 >= 0) {
            D_8012E700[ent->slot].flags |= 0x80000000;
        } else if (D_80140418 != 0 || (car->flagsE8 & 8)) {
            D_8012E700[ent->slot].flags ^= 0x80000000;
        } else {
            D_8012E700[ent->slot].flags &= 0x7FFFFFFF;
        }
        if (car->lightMode != 1 || D_80151AD0 >= 2) {
            model_data_load((s16) wheelA, 1, 15);
            return;
        }
        {
            ModelSlot *sa = &D_8012E700[wheelA];
            ModelSlot *sb = &D_8012E700[(s16) wheelA];
            u16 t = D_801427C0[(s16) (carIdx * 3 + 1)];
            if (sa->tex != t) {
                sb->tex = t;
            }
            if ((0x100 << car->lightIdx) & sb->flags) {
                model_transform_setup(wheelA, 0, 1 << car->lightIdx);
            }
            if (D_80140418 != 0 || (car->flagsE8 & 8)) {
                sa->flags ^= 0x80000000;
                return;
            }
            sa->flags &= 0x7FFFFFFF;
        }
    }
}

/* per-player light/model setup; the car index arrives in $s5 (IPA) */
void entity_tick_main(s16 ipa_s5, s32 flag)
{
    /* the retail frame is 368 bytes: these live at sp+324..356 and sp+24..324 is never
       touched (see STATUS.md); unreferenced locals do not reserve space at -O3 */
    struct {
        f32 v[3];
        s32 pad2;
        Color col;
        Node *nodes[3];
    } L;
    CarModels *mdl;
    Car *car;
    CarState *st;
    Node *src;
    s32 big;
    s32 a3;
    f32 K;

    L.col.w = D_8011B464;
    if (flag != 0) {
        mdl = &D_80139320[ipa_s5];
        model_data_load(mdl->id[1], 1, 15);
        model_data_load(mdl->id[2], 1, 15);
        model_data_load(mdl->id[3], 1, 15);
        return;
    }
    mdl = &D_80139320[ipa_s5];
    model_transform_setup(mdl->id[1], 0, 15);
    model_transform_setup(mdl->id[2], 0, 15);
    model_transform_setup(mdl->id[3], 0, 15);
    if (D_8014A110 == 2 && ipa_s5 > 0) {
        L.col.b[3] = 0x80;
    } else if (D_8014A110 == 6) {
        car = &D_80152818[ipa_s5];
        if (car->f38C & 1) {
            L.col.b[3] = car->b3A1;
        }
    }
    D_8012E700[(s16) mdl->id[1]].colA = L.col.w;
    D_8012E700[(s16) mdl->id[2]].colA = L.col.w;
    D_8012E700[(s16) mdl->id[3]].colA = L.col.w;

    a3 = (s16) mdl->id[0];
    K = D_80123A00;
    car = &D_80152818[ipa_s5];
    src = D_8012E700[a3].node;
    L.v[0] = 2.0f * src->m[3] + src->t[0];
    L.v[1] = 2.0f * src->m[4] + src->t[1];
    L.v[2] = 2.0f * src->m[5] + src->t[2];
    L.v[0] = src->m[6] * K + L.v[0];
    L.v[1] = src->m[7] * K + L.v[1];
    L.v[2] = src->m[8] * K + L.v[2];

    L.nodes[0] = D_8012E700[(s16) mdl->id[1]].node;
    math_utility(src, L.nodes[0]);
    L.nodes[0]->t[0] = L.v[0];
    L.nodes[0]->t[1] = L.v[1];
    L.nodes[0]->t[2] = L.v[2];
    L.nodes[1] = D_8012E700[(s16) mdl->id[2]].node;
    math_utility(src, L.nodes[1]);
    L.nodes[1]->t[0] = L.v[0];
    L.nodes[1]->t[1] = L.v[1];
    L.nodes[1]->t[2] = L.v[2];
    L.nodes[2] = D_8012E700[(s16) mdl->id[3]].node;
    math_utility(src, L.nodes[2]);
    L.nodes[2]->t[0] = L.v[0];
    L.nodes[2]->t[1] = L.v[1];
    L.nodes[2]->t[2] = L.v[2];

    big = 0;
    if (D_8014A110 == 6 && car->b384 == 3) {
        big = 1;
        car->scale[0] = D_80123A04;
        car->scale[1] = D_80123A04;
        car->scale[2] = D_80123A08;
    } else if ((car->sF8 >> 2) < 0) {
        car->scale[0] = 0.25f - (f32) RAND15() * D_80123A0C / 32768.0f;
        car->scale[1] = 0.25f - (f32) RAND15() * D_80123A0C / 32768.0f;
        car->scale[2] = D_80123A14 - (f32) RAND15() * D_80123A10 / 32768.0f;
    } else {
        st = &D_8014A250[ipa_s5];
        if (st->f3D0 < D_80123A18) {
            f32 lim = D_80123A1C;
            f32 f16, f2, f22;
            if (car->scale[0] < lim) {
                car->scale[0] = 0.25f - (f32) RAND15() * D_80123A20 / 32768.0f;
                f22 = D_80123A24;
                f16 = D_80123A28;
                f2 = D_80123A2C;
            } else {
                f16 = D_80123A30;
                f2 = D_80123A34;
                f22 = D_80123A38;
                car->scale[0] = car->scale[0] - ((f32) RAND15() * f16 / 32768.0f + f22);
                if (car->scale[0] < f2) {
                    car->scale[0] = f2;
                }
            }
            if (car->scale[1] < lim) {
                car->scale[1] = 0.25f - (f32) RAND15() * D_80123A3C / 32768.0f;
            } else {
                car->scale[1] = car->scale[1] - ((f32) RAND15() * f16 / 32768.0f + f22);
                if (car->scale[1] < f2) {
                    car->scale[1] = f2;
                }
            }
            if (car->scale[2] < D_80123A40) {
                car->scale[2] = D_80123A48 - (f32) RAND15() * D_80123A44 / 32768.0f;
            } else {
                car->scale[2] = car->scale[2] - ((f32) RAND15() * f16 / 32768.0f + f22);
                if (car->scale[2] < 0.5f) {
                    car->scale[2] = 0.5f;
                }
            }
        } else {
            f32 f14 = (f32) st->s7D0;
            f32 f22 = D_80123A50;
            f32 f12 = f14 * D_80123A54 / f22 + D_80123A4C;
            f32 f12b;
            if (f12 > 1.0f) {
                f12 = 1.0f;
            }
            car->scale[0] = f12 - (f32) RAND15() * D_80123A58 / 32768.0f;
            f12b = f14 / f22 + 0.5f;
            car->scale[1] = f12 - (f32) RAND15() * D_80123A58 / 32768.0f;
            if (f12b > 1.0f) {
                f12b = 1.0f;
            }
            car->scale[2] = f12b - (f32) RAND15() * D_80123A5C / 32768.0f;
        }
    }
    L.nodes[0]->m[6] = L.nodes[0]->m[6] * car->scale[0];
    L.nodes[0]->m[7] = L.nodes[0]->m[7] * car->scale[0];
    L.nodes[0]->m[8] = L.nodes[0]->m[8] * car->scale[0];
    L.nodes[1]->m[6] = L.nodes[1]->m[6] * car->scale[1];
    L.nodes[1]->m[7] = L.nodes[1]->m[7] * car->scale[1];
    L.nodes[1]->m[8] = L.nodes[1]->m[8] * car->scale[1];
    if (big != 0) {
        func_8008B32C(L.nodes[2], L.nodes[2], car->scale[2]);
    } else {
        L.nodes[2]->m[0] = L.nodes[2]->m[0] * car->scale[2];
        L.nodes[2]->m[1] = L.nodes[2]->m[1] * car->scale[2];
        L.nodes[2]->m[2] = L.nodes[2]->m[2] * car->scale[2];
        L.nodes[2]->m[3] = L.nodes[2]->m[3] * car->scale[2];
        L.nodes[2]->m[4] = L.nodes[2]->m[4] * car->scale[2];
        L.nodes[2]->m[5] = L.nodes[2]->m[5] * car->scale[2];
    }
    if (D_8014A110 == 6 && D_8014A250[ipa_s5].s6C4 >= 0) {
        D_8012E700[mdl->id[1]].flags |= 0x80000000;
        D_8012E700[mdl->id[2]].flags |= 0x80000000;
        D_8012E700[mdl->id[3]].flags |= 0x80000000;
        return;
    }
    if (D_80140418 != 0 || (car->flagsE8 & 8)) {
        D_8012E700[mdl->id[1]].flags ^= 0x80000000;
        D_8012E700[mdl->id[2]].flags ^= 0x80000000;
        D_8012E700[mdl->id[3]].flags ^= 0x80000000;
    } else {
        D_8012E700[mdl->id[1]].flags &= 0x7FFFFFFF;
        D_8012E700[mdl->id[2]].flags &= 0x7FFFFFFF;
        D_8012E700[mdl->id[3]].flags &= 0x7FFFFFFF;
    }
}
