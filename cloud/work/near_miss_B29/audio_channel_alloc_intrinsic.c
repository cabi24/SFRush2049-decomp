/* flags: -g0 -O2 -mips2 -G 0 -non_shared -Wab,-r4300_mul */
typedef signed char s8; typedef unsigned char u8; typedef signed short s16; typedef unsigned short u16; typedef signed int s32; typedef unsigned int u32; typedef float f32;
#define M2C_FIELD(e,t,o) (*(t)((u8 *)(e)+(o)))
extern f32 sqrtf(f32);
#pragma intrinsic(sqrtf)
extern u8 D_80151CE8[];
extern u16 D_801407F0; extern s32 D_801407F4;
extern s32 func_800B9F60(s32,s32,s32,s32*);
f32 audio_channel_alloc(s16 arg0, s32 arg1) {
    s32 sp70;
    f32 sp64;
    f32 sp60;
    f32 sp5C;
    f32 temp_f10;
    f32 temp_f18;
    f32 temp_f18_2;
    f32 temp_f6;
    f32 var_f20;
    s16 *temp_v1;
    s16 *temp_v1_2;
    s16 temp_t3;
    s16 temp_t7;
    s16 var_s0;
    s16 var_s0_2;
    s32 temp_a0;
    s32 temp_a0_2;
    s32 var_s1;
    s32 var_s1_2;
    u16 *var_s4;
    u16 var_s5;
    void *temp_v0;
    void *temp_v0_2;

    if ((arg1 < arg0) && (arg1 < M2C_FIELD(((s32 *) ((u8 *) &D_80151CE8 + (M2C_FIELD(&D_80151CE8, s16 *, 2) * 0x50))), s16 *, 0x2E))) {
        return -1.0f;
    }
    if (arg1 < arg0) {
        var_s4 = &D_801407F0;
        var_s5 = D_801407F0;
    } else {
        var_s4 = &D_801407F0;
        var_s5 = (u16) arg1;
    }
    var_s0 = arg0;
    var_f20 = 0.0f;
    if (arg0 < (s32) var_s5) {
        var_s1 = arg0 * 6;
        do {
            func_800B9F60(-1, var_s0, 0, &sp70);
            var_s4 = (u16 *) &D_801407F4;
            temp_a0 = *(u16 *) &D_801407F4;
            var_s0 += 1;
            temp_v1 = temp_a0 + var_s1;
            temp_t3 = M2C_FIELD(temp_v1, s16 *, 0);
            var_s1 += 6;
            temp_v0 = temp_a0 + (sp70 * 6);
            temp_f6 = (f32) (M2C_FIELD(temp_v0, s16 *, 0) - temp_t3);
            sp5C = temp_f6;
            sp60 = (f32) (M2C_FIELD(temp_v0, s16 *, 2) - M2C_FIELD(temp_v1, s16 *, 2));
            sp60 = 0.0f;
            temp_f18 = (f32) (M2C_FIELD(temp_v0, s16 *, 4) - M2C_FIELD(temp_v1, s16 *, 4));
            sp64 = temp_f18;
            var_f20 += sqrtf((temp_f18 * temp_f18) + ((temp_f6 * temp_f6) + (sp60 * sp60)));
        } while (var_s0 != var_s5);
    }
    if (var_s5 != arg1) {
        var_s0_2 = M2C_FIELD(((s32 *) ((u8 *) &D_80151CE8 + (M2C_FIELD(&D_80151CE8, s16 *, 2) * 0x50))), s16 *, 0x2E);
        if (var_s0_2 < arg1) {
            var_s1_2 = var_s0_2 * 6;
            do {
                func_800B9F60(-1, var_s0_2, 0, &sp70);
                temp_a0_2 = *var_s4;
                var_s0_2 += 1;
                temp_v1_2 = temp_a0_2 + var_s1_2;
                temp_t7 = M2C_FIELD(temp_v1_2, s16 *, 0);
                var_s1_2 += 6;
                temp_v0_2 = temp_a0_2 + (sp70 * 6);
                temp_f10 = (f32) (M2C_FIELD(temp_v0_2, s16 *, 0) - temp_t7);
                sp5C = temp_f10;
                sp60 = (f32) (M2C_FIELD(temp_v0_2, s16 *, 2) - M2C_FIELD(temp_v1_2, s16 *, 2));
                sp60 = 0.0f;
                temp_f18_2 = (f32) (M2C_FIELD(temp_v0_2, s16 *, 4) - M2C_FIELD(temp_v1_2, s16 *, 4));
                sp64 = temp_f18_2;
                var_f20 += sqrtf((temp_f18_2 * temp_f18_2) + ((temp_f10 * temp_f10) + (sp60 * sp60)));
            } while (var_s0_2 < arg1);
        }
    }
    return var_f20;
}
