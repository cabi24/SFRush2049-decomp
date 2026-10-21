/* flags: -g0 -O3 -mips2 -G 0 -non_shared */
typedef signed char s8; typedef unsigned char u8;
typedef short s16; typedef unsigned short u16; typedef int s32; typedef unsigned int u32; typedef float f32;
typedef struct SelectCarWave { s32 wave; s8 loading; s8 slot; u8 pad[6]; } SelectCarWave; /* anon_wave_loading_c */

extern SelectCarWave D_803BA230[16];   /* gSelectCarWavePtrs */
extern s16 D_80142B08[];               /* gCarModelNums */
extern s32 audio_frame_sync(s32, s32, s32, s32, s32); /* LoadWave */
extern void func_800BB02C(s32, s32, s32);

s32 func_80390BC0(s16 slot, s16 index, s32 *waveInst)
{
    s32 i;
    for (i = 0; i < 16; i++)
        if (D_803BA230[i].slot == slot && D_803BA230[i].loading) {
            *waveInst = D_803BA230[i].wave;
            return 1;
        }
    for (i = 0; i < 16; i++)
        if (D_803BA230[i].slot == slot)
            break;
    if (i == 16) {
        for (i = 0; i < 16; i++)
            if (D_803BA230[i].slot < 0)
                break;
        if (i == 16)
            return 0;
        D_803BA230[i].slot = slot;
    }
    *waveInst = D_803BA230[i].wave = audio_frame_sync(index + 88, 1, 1, 0, 0);
    D_803BA230[i].loading = 1;
    D_80142B08[slot] = D_803BA230[i].wave;
    func_800BB02C(slot, index, 0);
    return 1;
}
