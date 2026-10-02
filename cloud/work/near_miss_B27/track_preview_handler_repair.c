/* flags: -g0 -O2 -mips2 -G 0 -non_shared -Wab,-r4300_mul */
typedef signed char s8; typedef signed short s16; typedef signed int s32; typedef unsigned char u8; typedef float f32;
#define M2C_FIELD(e,t,o) (*(t)((u8 *)(e)+(o)))
extern f32 D_80124138,D_8012413C,D_80124140,D_80124144;
void track_preview_handler(void *arg0, void *arg1, f32 arg2) {
    f32 sp10;
    f32 temp_f0;
    f32 temp_f14;
    f32 temp_f14_2;
    f32 temp_f16;
    f32 temp_f16_2;
    f32 temp_f18;
    f32 temp_f20;

    temp_f0 = M2C_FIELD(arg1, f32 *, 0x10);
    temp_f18 = M2C_FIELD(arg1, f32 *, 0xC);
    M2C_FIELD(arg1, f32 *, 0x18) = (f32) (((M2C_FIELD(arg0, f32 *, 0x5BC) * arg2) / M2C_FIELD(arg0, f32 *, 0x5C8)) * 0.5f);
    temp_f14 = temp_f0 * 3.0f;
    temp_f16 = M2C_FIELD(arg1, f32 *, 0x18);
    temp_f20 = temp_f18 / temp_f16;
    M2C_FIELD(arg1, f32 *, 0x20) = temp_f20;
    M2C_FIELD(arg1, f32 *, 0x1C) = (f32) ((temp_f14 * temp_f16) / temp_f18);
    sp10 = temp_f20 * temp_f20;
    M2C_FIELD(arg1, f32 *, 0x24) = (f32) (sp10 / temp_f14);
    M2C_FIELD(arg1, f32 *, 0x2C) = (f32) (2.0f * M2C_FIELD(arg1, f32 *, 0x24));
    M2C_FIELD(arg1, f32 *, 0x28) = (f32) ((sp10 * temp_f20) / (27.0f * temp_f0 * temp_f0));
    M2C_FIELD(arg1, f32 *, 0x30) = (f32) (M2C_FIELD(arg1, f32 *, 0x28) * 3.0f);
    temp_f14_2 = D_80124138 / temp_f0;
    temp_f16_2 = temp_f14_2 * temp_f14_2;
    M2C_FIELD(arg1, f32 *, 0x34) = temp_f14_2;
    M2C_FIELD(arg1, f32 *, 0x38) = (f32) (temp_f16_2 * D_8012413C);
    M2C_FIELD(arg1, f32 *, 0x3C) = (f32) (temp_f16_2 * temp_f14_2 * D_80124140);
    M2C_FIELD(arg1, f32 *, 0x44) = 0.0f;
    M2C_FIELD(arg1, f32 *, 0x40) = (f32) D_80124144;
}
