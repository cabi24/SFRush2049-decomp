#include <assert.h>
#include <stddef.h>
#include <string.h>
#ifndef CANDIDATE
#define CANDIDATE "../../../matches/stat_lap_split.c"
#endif
#include CANDIDATE

s8 D_8010FFC0;
u8 D_80153E8F[64];
GameCar player_array[8];
s16 D_8011F020[16], D_8011F040[16];
static u32 captured[7], sound_calls, matrix_calls, return_tag;

static u32 float_bits(f32 value) {
    u32 result;
    memcpy(&result, &value, sizeof(result));
    return result;
}

/* Exact arithmetic and operand ordering of accepted func_800A61B0. */
void func_800A61B0(void *input, void *output, void *basis) {
    f32 *arg0 = (f32 *)input;
    f32 *arg1 = (f32 *)output;
    f32 *arg2 = (f32 *)basis;
    matrix_calls++;
    arg1[0] = (arg0[0] * arg2[0] + arg0[1] * arg2[1]) + arg0[2] * arg2[2];
    arg1[1] = (arg0[0] * arg2[3] + arg0[1] * arg2[4]) + arg0[2] * arg2[5];
    arg1[2] = (arg0[0] * arg2[6] + arg0[1] * arg2[7]) + arg0[2] * arg2[8];
}

u32 high_scores_display(u32 sound, u32 slot, u32 value, u8 mode,
                       f32 scale, f32 x, f32 y) {
    sound_calls++;
    captured[0]=sound; captured[1]=slot; captured[2]=value; captured[3]=mode;
    captured[4]=float_bits(scale); captured[5]=float_bits(x); captured[6]=float_bits(y);
    return return_tag;
}

void run_case(s32 enabled, s32 state, s32 sound, s32 slot, u32 mode,
              const f32 *data, u32 tag, const s16 *xs, const s16 *ys, u32 *out) {
    GameCar before[8];
    f32 position[3], original[3];
    s32 i;
    assert(sizeof(GameCar)==952 && offsetof(GameCar, position)==8);
    assert(offsetof(GameCar, matrix)==44 && slot>=0 && slot<8);
    memset(player_array, 0xa5, sizeof(player_array));
    memset(D_80153E8F, 0x5a, sizeof(D_80153E8F));
    memset(captured, 0, sizeof(captured));
    D_8010FFC0=(s8)enabled;
    D_80153E8F[slot*8]=(u8)state;
    for(i=0;i<3;i++) {
        position[i]=original[i]=data[i];
        player_array[slot].position[i]=data[3+i];
    }
    for(i=0;i<9;i++) player_array[slot].matrix[i]=data[6+i];
    for(i=0;i<16;i++) { D_8011F020[i]=xs[i]; D_8011F040[i]=ys[i]; }
    memcpy(before, player_array, sizeof(before));
    sound_calls=matrix_calls=0;
    return_tag=tag;
    out[0]=(u32)stat_lap_split(sound,slot,position,(u8)mode);
    out[1]=sound_calls;
    for(i=0;i<7;i++) out[2+i]=captured[i];
    out[9]=matrix_calls;
    assert(memcmp(before,player_array,sizeof(before))==0);
    assert(memcmp(original,position,sizeof(original))==0);
    assert(D_8010FFC0==(s8)enabled && D_80153E8F[slot*8]==(u8)state);
    for(i=0;i<16;i++) assert(D_8011F020[i]==xs[i] && D_8011F040[i]==ys[i]);
}
