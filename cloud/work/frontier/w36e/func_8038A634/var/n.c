/* flags: -g0 -O3 -mips2 -G 0 -non_shared */
typedef signed char s8; typedef unsigned char u8;
typedef short s16; typedef unsigned short u16; typedef int s32; typedef unsigned int u32; typedef float f32;
typedef union tRGBA { u8 c[4]; u32 w; } tRGBA;
typedef struct Palette { char name[16]; u8 start; u8 end; u16 format; char *palBase; } Palette;
typedef struct Blit {
    const char *name; Palette *palette; void *texture; u16 texIndex; s16 x; s16 y; u16 z;
    s16 width; s16 height; u8 alpha; u8 flip; s8 hide; s8 init;
} Blit;

extern s8 D_801427A0;             /* gFogColor */
extern s8 D_8014978C;             /* gTrackNum */
extern u8 D_80114658[][3];        /* gFogColors */
extern s8 D_8013F1D8;             /* gFogIntensity */
extern Blit *D_803B828C;          /* trackFogPoly (an N64 blit) */
extern void Input_ApplyPadConfig(Blit *); /* UpdateBlit */

void func_8038A634(s32 now)
{
    static tRGBA fogColor[2];
    static Palette n64FogPal;
    tRGBA col;

    if (D_801427A0 < 19)
        D_801427A0 = D_8014978C;
    col.c[0] = D_80114658[D_801427A0][0];
    col.c[1] = D_80114658[D_801427A0][1];
    col.c[2] = D_80114658[D_801427A0][2];
    if (now) {
        fogColor[0].c[0] = col.c[0];
        fogColor[0].c[1] = col.c[1];
        fogColor[0].c[2] = col.c[2];
    } else {
        fogColor[0].c[0] += (col.c[0] - fogColor[0].c[0]) / 10;
        fogColor[0].c[1] += (col.c[1] - fogColor[0].c[1]) / 10;
        fogColor[0].c[2] += (col.c[2] - fogColor[0].c[2]) / 10;
    }
    fogColor[0].c[3] = D_8013F1D8 * 192 / 3;
    if (D_803B828C) {
        n64FogPal.palBase = (char *)fogColor;
        fogColor[1].c[0] = 0;
        fogColor[1].c[1] = 0;
        fogColor[1].c[2] = 0;
        fogColor[1].c[3] = 0;
        D_803B828C->z = 0x7e00;
        D_803B828C->alpha = fogColor[0].c[3];
        D_803B828C->palette = &n64FogPal;
        Input_ApplyPadConfig(D_803B828C);
    }
}
