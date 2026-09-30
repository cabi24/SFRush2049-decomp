/*
 * Per-car rubber-banding ("render_large_objects" at 0x800F93A0) and its two
 * IPA callees. Hand-written from the assembly.
 */
float fabsf(float);
#pragma intrinsic (fabsf)

typedef signed char s8;
typedef unsigned char u8;
typedef signed short s16;
typedef unsigned short u16;
typedef signed int s32;
typedef float f32;

typedef struct {
    u8 pad0[7];
    u8 flag;
} Slot;

typedef struct {
    /* 0x000 */ u8 pad0[0x400];
    /* 0x400 */ f32 scale;
    /* 0x404 */ u8 pad404[0x7C6 - 0x404];
    /* 0x7C6 */ s16 order;
    /* 0x7C8 */ s16 active;
    /* 0x7CA */ s16 pad7CA;
    /* 0x7CC */ s8 kind;
    /* 0x7CD */ u8 pad7CD[0x7E6 - 0x7CD];
    /* 0x7E6 */ s16 rank;
    /* 0x7E8 */ u8 pad7E8[4];
    /* 0x7EC */ f32 outA;
    /* 0x7F0 */ f32 outB;
    /* 0x7F4 */ u8 pad7F4[0x808 - 0x7F4];
} Car;

typedef struct {
    /* 0x000 */ u8 pad0[8];
    /* 0x008 */ f32 pos[3];
    /* 0x014 */ u8 pad14[0xEE - 0x14];
    /* 0x0EE */ s8 place;
    /* 0x0EF */ u8 pad0EF[0x100 - 0xEF];
    /* 0x100 */ f32 dist;
    /* 0x104 */ u8 pad104[0x356 - 0x104];
    /* 0x356 */ s16 slot;
    /* 0x358 */ u8 pad358;
    /* 0x359 */ s8 state;
    /* 0x35A */ u8 pad35A[0x3B8 - 0x35A];
} Rec;

extern Car D_8014A250[];
extern Rec player_array[];
extern s16 active_player_count;
extern s8 D_80152030;
extern s8 D_80150F14;
extern Slot D_80153E88[];
extern f32 D_80124310;
extern f32 D_80124314;
extern f32 D_80124318;
extern f32 D_8012431C;
extern f32 D_80124638;

f32 func_800F92C8(f32 a, f32 b, f32 x, f32 outA, f32 outB);
void func_800DE860(void);

f32 func_800F92C8(f32 a, f32 b, f32 x, f32 outA, f32 outB)
{
    f32 d;
    f32 t;

    if (a < b) {
        if (x < a) return outA;
        if (b < x) return outB;
    } else {
        if (x < b) return outB;
        if (a < x) return outA;
    }
    d = a - b;
    if (fabsf(d) < D_80124638) {
        return (outA + outB) * 0.5f;
    }
    t = (outA - outB) / d;
    return t * x + (outA - t * a);
}

void func_800DE860(void)
{
    s16 i;
    s16 best;
    s32 j;
    f32 s;
    f32 d;
    f32 bd;
    f32 k1;
    f32 k2;

    if ((active_player_count == 1 && D_80152030 < 5) || D_80150F14 == 0) {
        for (j = 0; j < 6; j++) {
            if (D_80153E88[j].flag == 0 || D_80153E88[j].flag == 6) {
                D_8014A250[j].scale = 1.0f;
            }
        }
    } else {
        best = -1;
        for (i = 0; i < 6; i++) {
            if (D_8014A250[i].active != 0 && player_array[i].state < 2 &&
                (D_8014A250[i].kind == 2 || D_80152030 == 5)) {
                if (best == -1) {
                    best = i;
                } else if (player_array[best].dist < player_array[i].dist) {
                    best = i;
                }
            }
        }
        k1 = D_80124310;
        k2 = D_80124314;
        bd = player_array[best].dist;
        for (i = 0; i < 6; i++) {
            if (D_8014A250[i].active != 0 && player_array[i].state < 2 &&
                (D_8014A250[i].kind == 2 || D_80152030 == 5)) {
                d = bd - player_array[i].dist;
                if (d > k2) {
                    s = k1 + 1.0f;
                } else {
                    s = d * k1 / k2 + 1.0f;
                }
                if (active_player_count == 1) {
                    s = (1.0f - s) * 0.5f + 1.0f;
                }
                D_8014A250[i].scale = D_8014A250[i].scale * D_80124318 + D_8012431C * s;
            }
        }
    }
}

void __standin_render_large_objects(void)
{
    func_800DE860();
    func_800F92C8(1.0f, 2.0f, 3.0f, 4.0f, 5.0f);
}

void __standin_b(void)
{
    func_800DE860();
    func_800F92C8(1.0f, 2.0f, 3.0f, 4.0f, 5.0f);
}
