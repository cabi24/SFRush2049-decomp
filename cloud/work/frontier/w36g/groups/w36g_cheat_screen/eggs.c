/* flags: -g0 -O3 -mips2 -G 0 -non_shared */
typedef signed char s8; typedef unsigned char u8; typedef short s16; typedef int s32; typedef unsigned int u32;
typedef float f32;
typedef struct Egg { u8 size; u8 pad[3]; u32 levels[20]; u32 edges[20]; } Egg;
extern Egg D_803B3460[];        /* gEggs */
extern Egg *D_803B4278;         /* gEgg */
extern f32 D_803B427C;          /* gNextTimeOut */
extern s32 D_803B4280;          /* gEggIdx */

void InitEgg(s32 egg)
{
    D_803B4278 = &D_803B3460[egg];
    D_803B427C = 0.0f;
    D_803B4280 = 0;
}
