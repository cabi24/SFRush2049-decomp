/* flags: -g0 -O2 -mips2 -G 0 -non_shared -Wab,-r4300_mul */
typedef float f32; typedef unsigned char u8;
#define M2C_FIELD(e,t,o) (*(t)((u8 *)(e)+(o)))
extern f32 sqrtf(f32);
extern f32 D_8012394C;
f32 func_8008E0B8(void *arg0) {
    f32 sp4;
    f32 temp_f12;
    f32 temp_f14;
    f32 temp_f16;
    f32 temp_f2;

    temp_f12 = M2C_FIELD(arg0, f32 *, 0);
    temp_f16 = M2C_FIELD(arg0, f32 *, 4);
    sp4 = M2C_FIELD(arg0, f32 *, 8);
    temp_f2 = sqrtf((temp_f12 * temp_f12) + (temp_f16 * temp_f16) + (sp4 * sp4));
    if (temp_f2 <= D_8012394C) {
        return 0.0f;
    }
    temp_f14 = 1.0f / temp_f2;
    M2C_FIELD(arg0, f32 *, 0) = (f32) (temp_f12 * temp_f14);
    M2C_FIELD(arg0, f32 *, 4) = (f32) (temp_f16 * temp_f14);
    M2C_FIELD(arg0, f32 *, 8) = (f32) (sp4 * temp_f14);
    return temp_f2;
}
