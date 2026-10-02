/* flags: -g0 -O2 -mips2 -G 0 -non_shared -Wab,-r4300_mul */
typedef signed char s8; typedef signed short s16; typedef signed int s32; typedef unsigned char u8; typedef float f32;
#define M2C_FIELD(e,t,o) (*(t)((u8 *)(e)+(o)))
extern void func_800E31D4(void *),func_800E2F00(void *),func_800E2C70(void *),func_800E2AC4(void *);
void func_800E32CC(void *arg0) {
    f32 temp_f0;
    f32 temp_f0_2;
    f32 temp_f0_3;
    f32 temp_f14;
    f32 temp_f12;
    f32 temp_f2;

    if (M2C_FIELD(arg0, s8 *, 0xA) != 0) {
        func_800E31D4(arg0);
    }
    func_800E2F00(arg0);
    func_800E2C70(arg0);
    M2C_FIELD(arg0, f32 *, 0x41C) = (f32) ((M2C_FIELD(arg0, f32 *, 0x530) + M2C_FIELD(arg0, f32 *, 0x58C)) * 0.5f);
    func_800E2AC4(arg0);
    temp_f12 = M2C_FIELD(arg0, f32 *, 0x8C);
    temp_f14 = M2C_FIELD(arg0, f32 *, 0x80);
    temp_f2 = temp_f12 + temp_f14;
    if ((M2C_FIELD(arg0, s8 *, 0x5A8) == 0) || (temp_f2 < 500.0f)) {
        temp_f0 = M2C_FIELD(arg0, f32 *, 0x410) * 0.5f;
        M2C_FIELD(arg0, f32 *, 0x3BC) = (f32) (M2C_FIELD(arg0, f32 *, 0x3BC) + temp_f0);
        M2C_FIELD(arg0, f32 *, 0x3C0) = (f32) (M2C_FIELD(arg0, f32 *, 0x3C0) + temp_f0);
    } else if (temp_f14 <= 0.0f) {
        M2C_FIELD(arg0, f32 *, 0x3BC) = 0.0f;
        M2C_FIELD(arg0, f32 *, 0x3C0) = (f32) (M2C_FIELD(arg0, f32 *, 0x3C0) + M2C_FIELD(arg0, f32 *, 0x410));
    } else if (temp_f12 <= 0.0f) {
        M2C_FIELD(arg0, f32 *, 0x3C0) = 0.0f;
        M2C_FIELD(arg0, f32 *, 0x3BC) = (f32) (M2C_FIELD(arg0, f32 *, 0x3BC) + M2C_FIELD(arg0, f32 *, 0x410));
    } else {
        temp_f0_2 = M2C_FIELD(arg0, f32 *, 0x410);
        M2C_FIELD(arg0, f32 *, 0x3BC) = (f32) (M2C_FIELD(arg0, f32 *, 0x3BC) + ((temp_f0_2 * temp_f14) / temp_f2));
        M2C_FIELD(arg0, f32 *, 0x3C0) = (f32) (M2C_FIELD(arg0, f32 *, 0x3C0) + ((temp_f0_2 * temp_f12) / temp_f2));
    }
    temp_f0_3 = M2C_FIELD(arg0, f32 *, 0x418) * 2.0f;
    M2C_FIELD(arg0, f32 *, 0x4FC) = temp_f0_3;
    M2C_FIELD(arg0, f32 *, 0x558) = temp_f0_3;
}
