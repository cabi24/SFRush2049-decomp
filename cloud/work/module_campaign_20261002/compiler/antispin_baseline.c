/* flags: -g0 -O2 -mips2 -G 0 -non_shared -Wab,-r4300_mul */
typedef float f32; typedef int s32; typedef short s16; typedef unsigned char u8;
#define M2C_FIELD(p,t,o) (*(t)((u8*)(p)+(o)))
extern f32 D_801243C0;
void func_800E1AA0(void *arg0) {
    f32 temp_f0;
    f32 temp_f0_2;
    f32 temp_f2;
    f32 var_f0;

    if (!(M2C_FIELD(arg0, s32 *, 0x7D4) & 0x10)) {
        if ((M2C_FIELD(arg0, s32 *, 0x60C) != 8) || (M2C_FIELD(arg0, s32 *, 0x610) != 8)) {
            var_f0 = M2C_FIELD(arg0, f32 *, 0x5AC) + (M2C_FIELD(arg0, f32 *, 0x3D4) * 0.5f);
            if (var_f0 > 1.0f) {
                var_f0 = 1.0f;
            }
            temp_f2 = M2C_FIELD(arg0, f32 *, 0x3F0);
            if (temp_f2 > 100.0f) {
                M2C_FIELD(arg0, f32 *, 0x140) = (f32) (M2C_FIELD(arg0, f32 *, 0x140) - (M2C_FIELD(arg0, f32 *, 0x720) * D_801243C0 * var_f0));
            } else {
                M2C_FIELD(arg0, f32 *, 0x140) = (f32) (M2C_FIELD(arg0, f32 *, 0x140) - (M2C_FIELD(arg0, f32 *, 0x720) * temp_f2 * 120.0f * var_f0));
            }
        }
        if (!(M2C_FIELD(arg0, f32 *, 0x48) < 0.0f)) {
            if (M2C_FIELD(arg0, s16 *, 0x7CA) != 0) {
                temp_f0 = M2C_FIELD(arg0, f32 *, 0x40);
                if ((temp_f0 * M2C_FIELD(arg0, f32 *, 0x50)) > 0.0f) {
                    M2C_FIELD(arg0, f32 *, 0x140) = (f32) (M2C_FIELD(arg0, f32 *, 0x140) - (temp_f0 * 100.0f));
                }
            } else {
                temp_f0_2 = M2C_FIELD(arg0, f32 *, 0x40);
                if (((temp_f0_2 * M2C_FIELD(arg0, f32 *, 0x50)) > 0.0f) && (M2C_FIELD(arg0, f32 *, 0x3D4) < 0.5f)) {
                    M2C_FIELD(arg0, f32 *, 0x140) = (f32) (M2C_FIELD(arg0, f32 *, 0x140) - (temp_f0_2 * 100.0f * M2C_FIELD(M2C_FIELD(arg0, void **, 4), f32 *, 0x24)));
                }
            }
        }
    }
}
