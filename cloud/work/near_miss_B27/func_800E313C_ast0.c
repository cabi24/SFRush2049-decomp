/* flags: -g0 -O2 -mips2 -G 0 -non_shared -Wab,-r4300_mul */
typedef signed char s8; typedef signed short s16; typedef signed int s32; typedef unsigned char u8; typedef float f32;
#define M2C_FIELD(e,t,o) (*(t)((u8 *)(e)+(o)))
extern f32 D_80110EBC[][6];
void func_800E313C(void *arg0, f32 arg1, f32 arg2) {
    M2C_FIELD(arg0, s16 *, 0x3F4) = 1;
    if (M2C_FIELD(arg0, s16 *, 0x3F4) < 4) {
loop_1:
        if (!((M2C_FIELD(arg0, f32 *, 0x41C) * ((M2C_FIELD(M2C_FIELD(arg0, void **, 4), f32 *, 0xC) * M2C_FIELD(M2C_FIELD(arg0, void **, 0), f32 *, 0xA4)) * D_80110EBC[M2C_FIELD(arg0,s8 *,9)][M2C_FIELD(arg0,s16 *,0x3F4)+1])) < arg1)) {
            M2C_FIELD(arg0, s16 *, 0x3F4) = (s16) (M2C_FIELD(arg0, s16 *, 0x3F4) + 1);
            if (M2C_FIELD(arg0, s16 *, 0x3F4) < 4) {
                goto loop_1;
            }
        }
    }
}
