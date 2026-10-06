/* flags: -g0 -O3 -mips2 -G 0 -non_shared -- NOT A MATCH: 249/382 words, 12 normalised rows (colouring).
 * func_80109468: split-screen minimap Blit AnimFunc (N64-only). See ../RESULTS.md for semantics, what was
 * found and the residual (i/6 in s1/s2 swapped, dx/dz/size registers, as1 hoisting `move a0,s0`). */
/* w7d: from w6c best.c (12 normalised rows) -> 7: the clear loop uses j/y instead of i/j, so i is a
 * main-loop-only variable (s2) and the hoisted constant 6 takes s1 as in retail. Residual: dx/dz/size/px/pz
 * colouring in the dot loop (retail dx a2, dz a3, size v1, px v0, pz a1) and the two `move a0,s0` hoists. */
typedef signed char s8;
typedef unsigned char u8;
typedef signed short s16;
typedef unsigned short u16;
typedef signed int s32;
typedef unsigned int u32;
typedef float f32;

typedef struct TexDef {
    /* 0x00 */ char name[16];
    /* 0x10 */ u16 width, height;
    /* 0x14 */ u8 pad14[4];
    /* 0x18 */ u8 *data;
} TexDef;

typedef struct Blit {
    /* 0x00 */ char *name;
    /* 0x04 */ void *image;
    /* 0x08 */ TexDef *info;
    /* 0x0C */ s16 texIndex;
    /* 0x0E */ s16 X;
    /* 0x10 */ s16 Y;
    /* 0x12 */ s16 pad12;
    /* 0x14 */ s16 Width;
    /* 0x16 */ s16 Height;
    /* 0x18 */ u8 Alpha;
    /* 0x19 */ u8 pad19;
    /* 0x1A */ s8 Hide;
    /* 0x1C */ s16 Top;
    /* 0x1E */ s16 Bot;
    /* 0x20 */ s16 Left;
    /* 0x22 */ s16 Right;
    /* 0x24 */ s32 pad24;
    /* 0x28 */ s32 AnimDTA;
    /* 0x2C */ u32 AnimID;
} Blit;

typedef struct PathPoint {
    s16 pos[3];
} PathPoint;

typedef struct PathSet {
    /* 0x0 */ u8 type;
    /* 0x1 */ u8 pad1[9];
    /* 0xA */ u16 count;
    /* 0xC */ PathPoint *points;
} PathSet; /* 0x10 */

typedef struct PathGraph {
    /* 0x0 */ u16 count;
    /* 0x4 */ PathPoint *points;
    /* 0x8 */ u8 numSets;
    /* 0xC */ PathSet *sets;
} PathGraph;

typedef struct {
    s32 x;
    s32 y;
} Pos;

extern s16 D_80151AD0;
extern s8 D_80156BDC;
extern s32 D_801161C4;
extern Pos D_801160A8[];
extern s16 D_801407B4[3];
extern s16 D_801407D4[3];
extern PathGraph D_801407F0;
extern PathPoint *D_801409E8;
extern s8 D_80140A04;

void Input_ApplyPadConfig(Blit *blt);
void func_800EF5B0(Blit *blt, char *name, s32 preserve);

s32 func_80109468(Blit *blt) {
    s32 dx;
    s32 w;
    s32 dz;
    s32 h;
    s32 offx;
    s32 offz;
    s32 i;
    s32 j;
    s32 x;
    s32 y;
    s32 px;
    s32 pz;
    s32 col;
    u8 *row;
    s32 hide;
    s32 k;
    s32 m;

    hide = D_80151AD0 >= 5 || D_80156BDC == 0;
    if (hide != blt->Hide) {
        blt->Hide = hide;
        Input_ApplyPadConfig(blt);
    }
    if (blt->Hide) {
        return 1;
    }
    if (!(blt->AnimID & 0x10)) {
        return 1;
    }
    if (D_80151AD0 == 4) {
        func_800EF5B0(blt, "MAP_MD", 0);
    }
    D_801161C4 = blt->Width;
    if (blt->AnimID & 0x10) {
        blt->AnimID = blt->AnimID & 0xF;
        blt->X = D_801160A8[D_80151AD0 - 1].x - D_801161C4 / 2;
        blt->Y = D_801160A8[D_80151AD0 - 1].y - D_801161C4 / 2;
        Input_ApplyPadConfig(blt);
    }
    dx = D_801407B4[0] - D_801407D4[0];
    dz = D_801407B4[2] - D_801407D4[2];
    if (dz < dx) {
        w = D_801161C4 - 8;
        h = dz * w / dx;
    } else {
        h = D_801161C4 - 8;
        w = dx * h / dz;
    }
    offx = (D_801161C4 - w - 8) / 2 + 4;
    offz = (D_801161C4 - h - 8) / 2 + 4;
    for (k = 0; k < D_801161C4; k++) {
        for (m = 0; m < D_801161C4 / 2; m++) {
            blt->info->data[k * D_801161C4 / 2 + m] = 0;
        }
    }
    for (i = 0; i < D_801407F0.count; i++) {
        if (i >= D_801407F0.count) {
            for (j = 0; j < D_801407F0.numSets; j++) {
                if (&D_801409E8[i] >= D_801407F0.sets[j].points && &D_801409E8[i] < D_801407F0.sets[j].points + D_801407F0.sets[j].count) {
                    break;
                }
            }
            if (D_801407F0.sets[j].type == 0) {
                i += D_801407F0.sets[j].count - 1;
                continue;
            }
        }
        px = (D_801407F0.points[i].pos[0] - D_801407D4[0]) * w / dx + offx;
        pz = (D_801407F0.points[i].pos[2] - D_801407D4[2]) * h / dz + offz;
        for (y = pz - 2; y <= pz + 2; y++) {
            for (x = px - 2; x <= px + 2; x++) {
                if (x >= 0 && x < D_801161C4 && y >= 0 && y < D_801161C4) {
                    col = D_80140A04 == 0 ? D_801161C4 - x - 1 : x;
                    row = blt->info->data + (D_801161C4 - y - 1) * D_801161C4 / 2 + col / 2;
                    if (x <= px - 2 || x >= px + 2 || y <= pz - 2 || y >= pz + 2) {
                        if (col & 1) {
                            if ((*row & 0xF) != 0xE) {
                                *row = (*row & 0xF0) | 0xD;
                            }
                        } else {
                            if ((*row & 0xF0) != 0xE0) {
                                *row = (*row & 0xF) | 0xD0;
                            }
                        }
                    } else if (col & 1) {
                        *row = (*row & 0xF0) | 0xE;
                    } else {
                        *row = (*row & 0xF) | 0xE0;
                    }
                }
            }
        }
    }
    return 1;
}
