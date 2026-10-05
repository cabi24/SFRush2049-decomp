/* flags: -g0 -O3 -mips2 -G 0 -non_shared */
/*
 * sfx_position_3d (historical label): N64 InitDynamicObjs (arcade game/visuals.c), extended.
 *   - converts a 32-entry RGB byte palette (D_801226C0) to RGBA5551 words with alpha set;
 *   - sets four bytes at D_80138664 (11, 5, 26, 16);
 *   - for_game == 0: return (select screens);
 *   - clears 3328 bytes at D_80139320; with D_801174B4 & 0x100 calls the overlay routine at
 *     0x80391470 and clears gObjList (D_801427C0, s16) entries 0..155, otherwise runs the
 *     car-part lookups per link slot (sfx_volume_set = the arcade car-part loop, 13 slots,
 *     model from D_80142B08) and clears D_8013FE90[slot];
 *   - looks up 24 rim textures (MBOX_FindTexture_Sub = func_800B24EC, err 0) and stores the
 *     TEXDEF pointer (table D_80151AE8[index >> 10] + (index & 0x3FF)) or 0;
 *   - arcade `for (; i < MAX_TRK_OBJS; ++i) gObjList[i] = MBOX_FindObject(TrkNames[i -
 *     NUM_DYN_OBJS])`, here one entry (156) via string_copy_format (MBOX_FindObject_Sub, WARN);
 *   - looks up the 10 spark/shadow/skid textures into D_80161368.
 * Shaping quirks (compile-affecting): the arcade declaration list `i, j, k, model` plus an
 * unused buffer (>= 13 bytes; `partName[16]` here) for the 112-byte frame with `tex` at sp+94;
 * the car-slot loop uses `i` and the lookup loops `k` (with one shared variable the slot
 * counter loses s0 to the lookup pointers); the clear loop uses `j`; `k != 157` (with `<`
 * the one-entry loop is controlled by the name pointer); sfx_volume_set is declared with
 * an s16 model so the caller loads it with lh.  Also matches at -O2.
 */
typedef signed char s8; typedef unsigned char u8; typedef signed short s16; typedef unsigned short u16;
typedef signed int s32; typedef unsigned int u32;

#define GPACK_RGBA5551(r, g, b, a) ((((r) << 8) & 0xf800) | (((g) << 3) & 0x7c0) | (((b) >> 2) & 0x3e) | ((a) & 0x1))

typedef struct TexDef { u8 pad[36]; } TexDef;
typedef struct TexTable { TexDef *defs; u32 pad; } TexTable;
typedef struct CarInfo { u8 pad0; u8 car; u8 pad2[6]; } CarInfo;
typedef struct Rgb { u8 r, g, b, a; } Rgb;
typedef struct Four { u8 a, b, c, d; } Four;

extern Rgb D_801226C0[32];
extern u32 D_8013F300[32];
extern Four D_80138664;
extern u8 D_80139320[];
extern u32 D_801174B4;
extern s16 D_801427C0[];
extern CarInfo D_80153E88[];
extern s16 D_80142B08[];
extern u8 D_8013FE90[];
extern char *D_8011B3A0[24];
extern TexDef *D_80143F68[24];
extern TexTable D_80151AE8[];
extern volatile u8 D_80140BDC;
extern char *D_8011B400[];
extern char *D_8011B404[10];
extern s16 D_80161368[10];

extern void *memset(void *, s32, u32);
extern void func_80391470(void);
extern void sfx_volume_set(s16 slot, s16 car, s16 model);
extern s16 string_copy_format(char *name, s8 lo, s8 hi, s32 err);
extern TexDef *func_800B24EC(char *name, s16 *index, s8 lo, s8 hi, s32 err);

void sfx_position_3d(s32 for_game) {
    s32 i, j, k, model;
    u16 tex;
    char partName[16];

    for (i = 0; i < 32; i++) {
        D_8013F300[i] = (u16)GPACK_RGBA5551(D_801226C0[i].r, D_801226C0[i].g, D_801226C0[i].b, 0) | 1;
    }
    D_80138664.a = 11;
    D_80138664.b = 5;
    D_80138664.c = 26;
    D_80138664.d = 16;
    if (!for_game) {
        return;
    }
    memset(D_80139320, 0, 3328);
    if (D_801174B4 & 0x100) {
        func_80391470();
        for (j = 0; j < 156; j++) {
            D_801427C0[j] = 0;
        }
    } else {
        for (i = 0; i < 13; i++) {
            sfx_volume_set(i, D_80153E88[i].car, D_80142B08[i]);
            D_8013FE90[i] = 0;
        }
    }
    for (k = 0; k < 24; k++) {
        if (func_800B24EC(D_8011B3A0[k], (s16 *)&tex, 0, D_80140BDC - 1, 0)) {
            D_80143F68[k] = &D_80151AE8[tex >> 10].defs[tex & 0x3FF];
        } else {
            D_80143F68[k] = 0;
        }
    }
    for (k = 156; k != 157; k++) {
        D_801427C0[k] = string_copy_format(D_8011B400[k - 156], 0, D_80140BDC - 1, 1);
    }
    for (k = 0; k < 10; k++) {
        func_800B24EC(D_8011B404[k], &D_80161368[k], 0, D_80140BDC - 1, 0);
    }
}
