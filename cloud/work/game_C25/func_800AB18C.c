/* flags: -g0 -O2 -mips2 -G 0 -non_shared -Wab,-r4300_mul */
#include "types.h"
#define M2C_FIELD(p,t,o) (*(t)((u8*)(p)+(o)))
extern s32 D_8011E76C;
extern f32 D_8011E7A8;
extern s16 D_8011E7F0;
extern s16 D_8011E810;
extern s8 D_8013F1D8;
extern s8 D_8014978C;
extern f32 D_80151AA0;
extern s32 D_8017A510;
extern s16 active_player_count;
extern f32 sqrtf(f32);
#pragma intrinsic (sqrtf)
void func_800AB18C(s32 arg0, void *arg1) {
    f32 temp_f0;
    f32 temp_f0_2;
    f32 temp_f0_3;
    f32 temp_f0_4;
    f32 temp_f12;
    f32 temp_f14;
    f32 temp_f18_2;
    f32 temp_f2;
    f32 temp_f2_2;
    f32 temp_f6;
    f32 var_f2;
    s16 temp_a1;
    s16 temp_a2;
    s16 temp_a3;
    s16 var_a2;
    s16 var_a3;
    s32 temp_f18;
    s32 var_a0;
    s32 var_a1;
    s32 var_t2;
    s32 var_t5;
    void *temp_v0;

    if (D_8013F1D8 != 0) {
        temp_v0 = (s32 *) ((arg0 * 0x48) + (u8 *) &D_8017A510);
        M2C_FIELD(temp_v0, u16 *, 0x42) = 0x3E8U;
        M2C_FIELD(temp_v0, s16 *, 0x40) = (s16) *((s16 *) ((u8 *) &D_8011E810 + ((active_player_count * 8) + (D_8013F1D8 * 2))));
        M2C_FIELD(temp_v0, f32 *, 0x3C) = (f32) *((s16 *) ((u8 *) &D_8011E7F0 + ((active_player_count * 8) + (D_8013F1D8 * 2))));
        D_80151AA0 = *((f32 *) ((u8 *) &D_8011E7A8 + ((active_player_count * 0x10) + (D_8013F1D8 * 4))));
        var_a1 = (&D_8011E76C)[D_8014978C];
        if (var_a1 != 0) {
            var_a3 = M2C_FIELD(var_a1, s16 *, 4);
            var_a0 = 0;
            var_a2 = 0x7FFF;
            if (var_a3 != 0) {
                do {
                    temp_f2 = (f32) M2C_FIELD(var_a1, s16 *, 0) - M2C_FIELD(arg1, f32 *, 0);
                    temp_f14 = (f32) M2C_FIELD(var_a1, s16 *, 2) - M2C_FIELD(arg1, f32 *, 8);
                    temp_f18 = (s32) sqrtf((temp_f2 * temp_f2) + (temp_f14 * temp_f14));
                    if (((s16) temp_f18 < var_a3) && ((s16) temp_f18 < var_a2)) {
                        var_a0 = var_a1;
                        var_a2 = (s16) temp_f18;
                    }
                    var_a3 = M2C_FIELD(var_a1, s16 *, 0x10);
                    var_a1 += 0xC;
                } while (var_a3 != 0);
            }
            if (var_a0 != 0) {
                temp_f2_2 = M2C_FIELD(temp_v0, f32 *, 0x3C);
                temp_f0 = (f32) M2C_FIELD(var_a0, s16 *, 4);
                if (temp_f0 <= temp_f2_2) {
                    var_f2 = (f32) var_a2 / temp_f0;
                    goto block_13;
                }
                temp_f0_2 = (f32) var_a2;
                temp_f12 = temp_f2_2 - (f32) M2C_FIELD(var_a0, s16 *, 0xA);
                if (temp_f0_2 < temp_f12) {
                    var_f2 = temp_f0_2 / temp_f12;
block_13:
                    temp_a1 = M2C_FIELD(var_a0, s16 *, 6);
                    temp_f6 = (f32) temp_a1 + (var_f2 * (f32) ((u16) M2C_FIELD(temp_v0, s16 *, 0x40) - temp_a1));
                    var_t5 = (u32)temp_f6;
                    M2C_FIELD(temp_v0, s16 *, 0x40) = (s16) var_t5;
                    temp_a2 = M2C_FIELD(var_a0, s16 *, 8);
                    temp_f18_2 = (f32) temp_a2 + (var_f2 * (f32) (M2C_FIELD(temp_v0, u16 *, 0x42) - temp_a2));
                    var_t2 = (u32)temp_f18_2;
                    temp_a3 = (var_t2 & 0xFFFF) - 4;
                    M2C_FIELD(temp_v0, u16 *, 0x42) = (u16) var_t2;
                    if (temp_a3 < (s32) (u16) M2C_FIELD(temp_v0, s16 *, 0x40)) {
                        M2C_FIELD(temp_v0, s16 *, 0x40) = temp_a3;
                    }
                    temp_f0_3 = (f32) M2C_FIELD(var_a0, s16 *, 0xA);
                    M2C_FIELD(temp_v0, f32 *, 0x3C) = (f32) (temp_f0_3 + (var_f2 * (M2C_FIELD(temp_v0, f32 *, 0x3C) - temp_f0_3)));
                    temp_f0_4 = (f32) M2C_FIELD(var_a0, s16 *, 0xA);
                    D_80151AA0 = temp_f0_4 + (var_f2 * (D_80151AA0 - temp_f0_4));
                }
            }
        }
    }
}
