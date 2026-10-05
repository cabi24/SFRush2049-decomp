/* flags: -g0 -O3 -mips2 -G 0 -non_shared */
/*
 * N64 per-player HUD indicator placement and visibility callback.
 * The shared Hidden helper follows arcade game/hud.c at revision
 * 845329d7b36f5a384c5625ed9a0aef584ab46139; the callback itself and four-player
 * position table are N64-specific. Blit fields follow LIB/blit.h with the
 * observed N64 prefix offsets. Input_ApplyPadConfig is the UpdateBlit adapter;
 * func_800EF5B0 is RenameBlit.
 *
 * D_801543CA uses the volatile signed-half declaration already present in
 * accepted src/blob/func_800EC914.c. This yields a strict whole-body match,
 * but independent asynchronous-writer/source-contract evidence is not yet
 * established. See this packet's README before treating it as admissible.
 * No invented work, local padding, artificial helper, or compiler knob is used.
 */
typedef signed char s8;
typedef unsigned char u8;
typedef signed short s16;
typedef unsigned short u16;
typedef signed int s32;
typedef unsigned int u32;
typedef float f32;
typedef struct Blit {
    const char *Name;
    void *image;
    void *Info;
    s16 TexIndex;
    s16 X, Y;
    u16 Z;
    s16 Width, Height;
    u8 Alpha, Flip;
    s8 Hide;
    u8 Init;
    s16 Top, Bot, Left, Right;
    s16 color;
    u16 unknown26;
    s32 (*AnimFunc)(struct Blit *);
    u32 AnimID;
} Blit;
typedef struct { u8 other0[239]; s8 mode; u8 other240[712]; } Object952;
extern Object952 D_80152818[];
extern s16 D_80151AD0;
extern volatile s16 D_801543CA;
extern s8 D_80156CE8;
extern s32 state_word_a, D_8011617C, D_80116180;
extern s32 D_80116028[][4][2];
extern char D_80120E34[];
extern void Input_ApplyPadConfig(Blit *);
extern void func_800EF5B0(Blit *, const char *, s32);
static s32 Hidden(Blit *blt, s32 hide)
{
    if (hide != blt->Hide) {
        blt->Hide = hide;
        Input_ApplyPadConfig(blt);
    }
    return blt->Hide;
}
s32 func_80108DA8(Blit *blt)
{
    s32 slot;
    slot = blt->AnimID;
    if (slot >= D_80151AD0 || (state_word_a & 8) || D_801543CA < 2) {
        blt->AnimFunc = 0;
        return Hidden(blt, 1);
    }
    if (Hidden(blt, !D_80156CE8 || D_80152818[slot].mode == 1))
        return 1;
    blt->X = D_80116028[D_80151AD0 - 1][slot][0];
    blt->Y = D_80116028[D_80151AD0 - 1][slot][1];
    if (D_80151AD0 >= 2) {
        func_800EF5B0(blt, D_80120E34, 0);
        blt->Alpha = 0x60;
    }
    Input_ApplyPadConfig(blt);
    D_8011617C = blt->Width;
    D_80116180 = blt->Height;
    return 1;
}
