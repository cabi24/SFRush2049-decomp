/* flags: -g0 -O2 -mips2 -G 0 -non_shared -Wab,-r4300_mul */
#include "types.h"
#define M2C_FIELD(p,t,o) (*(t)((u8*)(p)+(o)))
extern f32 D_801244D4;extern s32 D_8012E638;extern s8 D_8012E670;extern f32 D_80152720;
extern f32 func_8008B3C8(f32 *);extern void func_800E8D50(void*,void*,s32,f32*);
void func_800E9C70(s16 arg0, void *arg1, void *arg2, s32 arg3) {
    f32 sp34;
    f32 sp30;
    f32 sp2C;
    f32 temp_f0;
    f32 temp_f2;
    s8 temp_v1;
    void *temp_v0;
    void *temp_v0_2;
    void *temp_v0_3;

    temp_v1 = M2C_FIELD(arg1, s8 *, 0x35C);
    if (arg0 == 0) {
        (&D_8012E670)[temp_v1] = 0;
        temp_v0 = (s32 *) ((temp_v1 * 0xC) + (u8 *) &D_8012E638);
        M2C_FIELD(temp_v0, f32 *, 0) = (f32) M2C_FIELD(arg2, f32 *, 0);
        M2C_FIELD(temp_v0, f32 *, 4) = (f32) M2C_FIELD(arg2, f32 *, 4);
        M2C_FIELD(temp_v0, f32 *, 8) = (f32) M2C_FIELD(arg2, f32 *, 8);
        return;
    }
    if (arg0 == 1) {
        (&D_8012E670)[temp_v1] = 1;
        temp_v0_2 = (s32 *) ((temp_v1 * 0xC) + (u8 *) &D_8012E638);
        M2C_FIELD(temp_v0_2, f32 *, 0) = (f32) M2C_FIELD(arg2, f32 *, 0);
        M2C_FIELD(temp_v0_2, f32 *, 4) = (f32) M2C_FIELD(arg2, f32 *, 4);
        M2C_FIELD(temp_v0_2, f32 *, 8) = (f32) M2C_FIELD(arg2, f32 *, 8);
        return;
    }
    temp_v0_3 = (s32 *) ((temp_v1 * 0xC) + (u8 *) &D_8012E638);
    sp2C = M2C_FIELD(temp_v0_3, f32 *, 0) - M2C_FIELD(arg2, f32 *, 0);
    sp30 = M2C_FIELD(temp_v0_3, f32 *, 4) - M2C_FIELD(arg2, f32 *, 4);
    (&D_80152720)[temp_v1] = 0.0f;
    sp34 = M2C_FIELD(temp_v0_3, f32 *, 8) - M2C_FIELD(arg2, f32 *, 8);
    if ((&D_8012E670)[temp_v1] != 0) {
        temp_f0 = func_8008B3C8(&sp2C);
        if (temp_f0 != 0.0f) {
            temp_f2 = (((temp_f0 - 50.0f) * D_801244D4) + 20.0f) / temp_f0;
            sp2C *= temp_f2;
            sp30 *= temp_f2;
            sp34 *= temp_f2;
        }
    }
    func_800E8D50(arg1, arg2, arg3, &sp2C);
}
