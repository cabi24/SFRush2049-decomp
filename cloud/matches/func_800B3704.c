/* flags: -g0 -O3 -mips2 -G 0 -non_shared */
/*
 * func_800B3704: arcade NewBlit (LIB/blit.c) with a fourth N64 argument.
 *   if (NumBlits >= MAXBLITS) return 0;   (arcade FatalError compiled out)
 *   blit = BlitPtrs[NumBlits++] (D_80149450 / D_80149788); Name, X, Y, Hide = Flip = 0, Init = 1,
 *   child = AnimFunc = 0, AnimID = AnimDTA = -1, data = 0; InitBlit (collision_sound_play);
 *   BLIdx = ZOID_AddBlit(TexIndex, 0, 0, x, y, -1, -1) (func_800A79F4, N64 adds the two NULLs);
 *   UpdateBlit (Input_ApplyPadConfig); return blit.
 * Arcade blit.h types matter: AnimID is U32 and AnimDTA S32, so the two -1 stores are two
 * different constants (separate `li`), not one shared register web.
 * Fourth parameter: every caller passes one (sound_control: `animid & 0x80000000`, ambient_sound_set: 0).
 * SHAPING DEVICE (disclosed): the compiled-out read `if (flags) {}` before the pop. It keeps the
 * fourth parameter's a3 web live over `name`, so name's copy is coloured t0 as in retail (forced-colour
 * oracle: name w14 -> t0 alone gives 0 rows; without the read name takes a3, 14 words). Probably a
 * compiled-out debug use of the flag. Placed right before `blit->name = name;` it also matches.
 * Whole-program unit: EQUAL.
 */
typedef unsigned short u16;
typedef signed short s16;
typedef unsigned char u8;
typedef signed char s8;
typedef signed int s32;

typedef struct Blit {
    /* 0x00 */ char *name;
    /* 0x04 */ void *image;
    /* 0x08 */ void *info;
    /* 0x0C */ u16 texIndex;
    /* 0x0E */ s16 x;
    /* 0x10 */ s16 y;
    /* 0x12 */ u16 state;
    /* 0x14 */ u16 width, height;
    /* 0x18 */ u8 alpha;
    /* 0x19 */ u8 flip;
    /* 0x1A */ u8 hide;
    /* 0x1B */ u8 init;
    /* 0x1C */ s16 top, bot, left, right, color;
    /* 0x26 */ s16 pad26;
    /* 0x28 */ void *animFunc;
    /* 0x2C */ unsigned int animID;
    /* 0x30 */ s32 animDTA;
    /* 0x34 */ s16 blIdx;
    /* 0x36 */ s16 pad36;
    /* 0x38 */ void *data;
    /* 0x3C */ struct Blit *child;
} Blit;

#define MAXBLITS 200

extern s32 D_80149788;          /* NumBlits */
extern Blit *D_80149450[];      /* BlitPtrs */

void collision_sound_play(Blit *blit);      /* InitBlit */
s32 func_800A79F4(s32 id, void *p4, void *p0, s32 a, s32 b, s32 w, s32 h); /* ZOID_AddBlit */
void Input_ApplyPadConfig(Blit *blit);      /* UpdateBlit */

Blit *func_800B3704(char *name, s32 x, s32 y, s32 flags)
{
    Blit *blit;

    if (D_80149788 >= MAXBLITS) {
        return 0;
    }

    if (flags) {}
    blit = D_80149450[D_80149788];
    D_80149788++;

    blit->name = name;
    blit->x = x;
    blit->y = y;
    blit->hide = 0;
    blit->flip = 0;
    blit->init = 1;
    blit->child = 0;
    blit->animFunc = 0;
    blit->animID = -1;
    blit->animDTA = -1;
    blit->data = 0;
    collision_sound_play(blit);
    blit->blIdx = func_800A79F4(blit->texIndex, 0, 0, x, y, -1, -1);
    Input_ApplyPadConfig(blit);
    return blit;
}
