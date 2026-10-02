/* flags: -g0 -O2 -mips2 -G 0 -non_shared -Wab,-r4300_mul */
#include "types.h"
#define M2C_FIELD(p,t,o) (*(t)((u8*)(p)+(o)))
extern s32 D_801141C8;
extern f32 D_8012388C;
extern f32 fabsf(f32),sqrtf(f32);
#pragma intrinsic (fabsf)
#pragma intrinsic (sqrtf)
extern f32 func_8008B3C8(void*);extern void vector_copy_scale(void*,void*);
#define M2C_BITWISE(t,x) (x)
void vector_normalize_length(void *arg0, void *arg1) {
    f32 temp_f0;
    f32 temp_f0_3;
    f32 temp_f12;
    f32 temp_f14;
    f32 temp_f16;
    f32 temp_f18;
    f32 temp_f22;
    f32 temp_f2;
    f32 temp_f2_2;
    f32 temp_f0_2;
    f32 temp_f20;
    void *temp_a0;

    M2C_FIELD(arg1, f32 *, 0x18) = (f32) M2C_FIELD(arg0, f32 *, 0);
    M2C_FIELD(arg1, f32 *, 0x1C) = (f32) M2C_FIELD(arg0, f32 *, 4);
    temp_a0 = (u8 *) arg1 + 0x18;
    M2C_FIELD(arg1, f32 *, 0x20) = M2C_FIELD(arg0, f32 *, 8);
    vector_copy_scale(temp_a0, temp_a0);
    M2C_FIELD(arg1, f32 *, 4) = 0.0f;
    M2C_FIELD(arg1, f32 *, 0) = M2C_FIELD(arg0, f32 *, 8);
    M2C_FIELD(arg1, f32 *, 8) = (f32) -M2C_FIELD(arg0, f32 *, 0);
    temp_f0 = M2C_BITWISE(f32, func_8008B3C8(arg1));
    if (temp_f0 <= D_8012388C) {
        M2C_FIELD(arg1, f32 *, 0) = M2C_FIELD(&D_801141C8, f32 *, 0);
        M2C_FIELD(arg1, f32 *, 4) = (f32) M2C_FIELD(&D_801141C8, f32 *, 4);
        M2C_FIELD(arg1, f32 *, 8) = (f32) M2C_FIELD(&D_801141C8, f32 *, 8);
    } else {
        temp_f2 = 1.0f / temp_f0;
        M2C_FIELD(arg1, f32 *, 0) = M2C_BITWISE(s32, (M2C_BITWISE(f32, M2C_FIELD(arg1, f32 *, 0)) * temp_f2));
        M2C_FIELD(arg1, f32 *, 4) = (f32) (M2C_FIELD(arg1, f32 *, 4) * temp_f2);
        M2C_FIELD(arg1, f32 *, 8) = (f32) (M2C_FIELD(arg1, f32 *, 8) * temp_f2);
    }
    temp_f18 = M2C_FIELD(arg1, f32 *, 0x1C);
    temp_f12 = M2C_FIELD(arg1, f32 *, 8);
    temp_f2_2 = M2C_FIELD(arg1, f32 *, 4);
    temp_f20 = M2C_FIELD(arg1, f32 *, 0x20);
    temp_f0_2 = M2C_FIELD(arg1, f32 *, 0);
    temp_f22 = M2C_FIELD(arg1, f32 *, 0x18);
    M2C_FIELD(arg1, f32 *, 0xC) = (f32) ((temp_f18 * temp_f12) - (temp_f2_2 * M2C_BITWISE(f32, temp_f20)));
    temp_f0_3 = M2C_FIELD(arg1, f32 *, 0xC);
    temp_f14 = (M2C_BITWISE(f32, temp_f20) * M2C_BITWISE(f32, temp_f0_2)) - (temp_f12 * temp_f22);
    temp_f16 = (temp_f22 * temp_f2_2) - (M2C_BITWISE(f32, temp_f0_2) * temp_f18);
    M2C_FIELD(arg1, f32 *, 0x10) = temp_f14;
    M2C_FIELD(arg1, f32 *, 0x14) = temp_f16;
    M2C_FIELD(arg1, f32 *, 0) = M2C_BITWISE(s32, ((temp_f14 * M2C_BITWISE(f32, temp_f20)) - (temp_f18 * temp_f16)));
    M2C_FIELD(arg1, f32 *, 4) = (f32) ((M2C_FIELD(arg1, f32 *, 0x14) * temp_f22) - (M2C_BITWISE(f32, temp_f20) * temp_f0_3));
    M2C_FIELD(arg1, f32 *, 8) = (f32) ((temp_f0_3 * temp_f18) - (temp_f22 * M2C_FIELD(arg1, f32 *, 0x10)));
}
