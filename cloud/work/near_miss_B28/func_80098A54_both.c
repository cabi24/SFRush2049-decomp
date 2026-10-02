/* flags: -g0 -O2 -mips2 -G 0 -non_shared -Wab,-r4300_mul */
typedef float f32; typedef unsigned char u8;
#define M2C_FIELD(e,t,o) (*(t)((u8 *)(e)+(o)))
extern f32 sqrtf(f32);
#pragma intrinsic(sqrtf)
f32 func_80098A54(f32 *arg0) {
    f32 temp_f0;
    f32 temp_f12;
    f32 temp_f14;
    f32 temp_f18;
    f32 temp_f2;

    temp_f2 = arg0[2];
    temp_f12 = arg0[0];
    temp_f14 = arg0[1];
    temp_f0 = sqrtf(((temp_f12 * temp_f12) + (temp_f14 * temp_f14)) + (temp_f2 * temp_f2));
    if (temp_f0 != 0.0f) {
        temp_f18 = 1.0f / temp_f0;
        arg0[0] = (f32) (temp_f12 * temp_f18);
        arg0[1] = (f32) (temp_f14 * temp_f18);
        arg0[2] = (f32) (temp_f2 * temp_f18);
    } else {
    arg0[1] = 0.0f;
    arg0[2] = 0.0f;
    arg0[0] = 1.0f;
    }
    return temp_f0;
}
