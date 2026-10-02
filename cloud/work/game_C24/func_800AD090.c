/* flags: -g0 -O2 -mips2 -G 0 -non_shared -Wab,-r4300_mul */
typedef float f32;
void func_800AD090(f32 arg0, f32 arg1, f32 *arg2) {
    f32 sp4;
    f32 temp_f16;
    f32 temp_f16_2;
    f32 temp_f18;
    f32 temp_f2;
    f32 temp_f2_2;

    temp_f2 = arg2[0];
    temp_f16 = arg2[3];
    temp_f18 = arg2[1];
    arg2[0] = (f32) ((temp_f2 * arg1) - (temp_f16 * arg0));
    arg2[3] = (f32) ((temp_f2 * arg0) + (temp_f16 * arg1));
    sp4 = arg2[4];
    temp_f2_2 = arg2[2];
    temp_f16_2 = arg2[5];
    arg2[1] = (f32) ((temp_f18 * arg1) - (sp4 * arg0));
    arg2[4] = (f32) ((temp_f18 * arg0) + (sp4 * arg1));
    arg2[2] = (f32) ((temp_f2_2 * arg1) - (temp_f16_2 * arg0));
    arg2[5] = (f32) ((temp_f2_2 * arg0) + (temp_f16_2 * arg1));
}
