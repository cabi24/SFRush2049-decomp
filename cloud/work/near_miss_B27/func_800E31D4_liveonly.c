/* flags: -g0 -O2 -mips2 -G 0 -non_shared -Wab,-r4300_mul */
typedef signed char s8; typedef signed short s16; typedef signed int s32; typedef unsigned char u8; typedef float f32;
#define M2C_FIELD(e,t,o) (*(t)((u8 *)(e)+(o)))
extern void func_800E313C(void *,f32,f32),func_800E30F4(void *),func_800E30AC(void *);
void func_800E31D4(void *arg0) {
    f32 sp18;
    f32 temp_f0;
    f32 temp_f12;
    f32 temp_f2;
    f32 var_f0;
    s16 temp_v0_2;
    s16 temp_v1;
    void *temp_v0;

    temp_v0 = M2C_FIELD(arg0, void **, 0);
    temp_f0 = (M2C_FIELD(arg0, f32 *, 0x3D0) + 3.0f) * 0.25f;
    temp_f2 = M2C_FIELD(M2C_FIELD(arg0, void **, 4), f32 *, 0x14);
    temp_f12 = temp_f2 * M2C_FIELD(temp_v0, f32 *, 0xAC) * temp_f0;
    sp18 = temp_f2 * M2C_FIELD(temp_v0, f32 *, 0xB0) * temp_f0;
    temp_v1 = M2C_FIELD(arg0, s16 *, 0x3F6);
    if ((temp_v1 == 0) || (temp_v1 == -1)) {
        M2C_FIELD(arg0, s16 *, 0x3F4) = temp_v1;
        return;
    }
    temp_v0_2 = M2C_FIELD(arg0, s16 *, 0x3F4);
    if ((temp_v0_2 == 0) || (temp_v0_2 == -1)) {
        func_800E313C(arg0, temp_f12, sp18);
    }
    var_f0 = M2C_FIELD(arg0, f32 *, 0x408);
    if (temp_f12 < var_f0) {
        func_800E30F4(arg0);
        var_f0 = M2C_FIELD(arg0, f32 *, 0x408);
    }
    if (var_f0 < sp18) {
        func_800E30AC(arg0);
    }
}
