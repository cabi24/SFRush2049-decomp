/* flags: -g0 -O3 -mips2 -G 0 -non_shared */
/* Runtime image A func_80390BC0 = LoadSelectCar (MAT3 GameCube GameSource selectcarload.c, LoadSelectCar,
 * GC 0x8003b97c, size 0x2a4; lane w36e, wave 36). Finds or allocates the car-select wave slot for `slot`, loads
 * the car model wave and records it in gCarModelNums.
 * N64 differences read from the retail words: the wave index is index + 88 (GC + 64), and the N64 build calls
 * func_800BB02C(slot, index, 0) after recording the model number. The loop bound is a local `count`
 * (retail keeps 16 in a register for the slt/bne loop tests instead of slti, and keeps the counters: no LFTR).
 * Code shape of the search loops from the earlier research packet
 * cloud/work/frontier/dot_runtime_a_resource_cache_20261006 (5 words off); the MAT3 chained assignment
 * `*waveInst = gSelectCarWavePtrs[i].wave = LoadWave(...)` and direct array indexing close it.
 */
typedef signed char s8;
typedef unsigned char u8;
typedef short s16;
typedef int s32;

typedef struct SelectCarWave {   /* anon_wave_loading_c */
    s32 wave;
    s8 loading;
    s8 slot;
    u8 pad[6];
} SelectCarWave;

extern SelectCarWave D_803BA230[];   /* gSelectCarWavePtrs[16] */
extern s16 D_80142B08[];             /* gCarModelNums */
extern s32 audio_frame_sync(s32 index, s32 a, s32 b, s32 c, void *d);   /* LoadWave */
extern void func_800BB02C(s32 slot, s32 index, void *data);

s32 func_80390BC0(s16 slot, s16 index, s32 *waveInst)
{
    s32 i;
    s32 count = 16;

    for (i = 0; i < count; i++) {
        if (D_803BA230[i].slot == slot && D_803BA230[i].loading != 0) {
            *waveInst = D_803BA230[i].wave;
            return 1;
        }
    }
    for (i = 0; i != count; i++) {
        if (D_803BA230[i].slot == slot)
            break;
    }
    if (i == count) {
        for (i = 0; i != count; i++) {
            if (D_803BA230[i].slot < 0)
                break;
        }
        if (i == count)
            return 0;
        D_803BA230[i].slot = slot;
    }
    *waveInst = D_803BA230[i].wave = audio_frame_sync(index + 88, 1, 1, 0, 0);
    D_803BA230[i].loading = 1;
    D_80142B08[slot] = D_803BA230[i].wave;
    func_800BB02C(slot, index, 0);
    return 1;
}
