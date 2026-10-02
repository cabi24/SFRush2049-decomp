/* flags: -g0 -O2 -mips2 -G 0 -non_shared -Wab,-r4300_mul */
typedef float f32; typedef unsigned char u8;
#define M2C_FIELD(e,t,o) (*(t)((u8 *)(e)+(o)))
extern f32 sqrtf(f32);
f32 func_80098A54(void *arg0) {
    f32 temp_f0;
    f32 temp_f12;
    f32 temp_f14;
    f32 temp_f18;
    f32 temp_f2;

    temp_f12 = M2C_FIELD(arg0, f32 *, 0);
    temp_f14 = M2C_FIELD(arg0, f32 *, 4);
    temp_f2 = M2C_FIELD(arg0, f32 *, 8);
    temp_f0 = sqrtf((temp_f2 * temp_f2) + ((temp_f12 * temp_f12) + (temp_f14 * temp_f14)));
    if (temp_f0 != 0.0f) {
        temp_f18 = 1.0f / temp_f0;
        M2C_FIELD(arg0, f32 *, 0) = (f32) (temp_f12 * temp_f18);
        M2C_FIELD(arg0, f32 *, 4) = (f32) (temp_f14 * temp_f18);
        M2C_FIELD(arg0, f32 *, 8) = (f32) (temp_f2 * temp_f18);
        return temp_f0;
    }
    M2C_FIELD(arg0, f32 *, 4) = 0.0f;
    M2C_FIELD(arg0, f32 *, 8) = 0.0f;
    M2C_FIELD(arg0, f32 *, 0) = 1.0f;
    return temp_f0;
}
