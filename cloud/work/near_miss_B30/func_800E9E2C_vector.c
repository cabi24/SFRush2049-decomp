/* flags: -g0 -O2 -mips2 -G 0 -non_shared -Wab,-r4300_mul */
typedef signed char s8; typedef int s32; typedef unsigned int u32; typedef float f32;
#define M2C_FIELD(expr,type,offset) (*(type)((char*)(expr)+(offset)))
extern f32 D_80110668,D_801543CC,D_801244D8,D_801244DC,D_801244E0,D_801244E4,D_80152720;
extern s8 D_80110680;
extern f32 cosf(f32),sinf(f32);
extern void func_800E8D50(void*,s32,s32,f32*);
void func_800E9E2C(void *arg0, s32 arg1, s32 arg2) {
    f32 vector[3];
    f32 sp30;
    s32 sp2C;
    f32 *temp_a1;
    f32 temp_f18;
    f32 temp_f6;
    f32 temp_f8;
    f32 var_f12;
    f32 var_f4;
    f32 var_f8;
    s32 var_t6;
    s32 var_t9;
    s8 temp_v0;
    s8 var_v1;
    u32 temp_hi;
    u32 temp_hi_2;

    sp2C = (s32) M2C_FIELD(arg0, s8 *, 0x35C);
    temp_v0 = M2C_FIELD(arg0, s8 *, 0x35B);
    var_v1 = (&D_80110680)[temp_v0];
    temp_a1 = &(&D_80110668)[temp_v0];
    if ((var_v1 == 1) && ((*temp_a1 + 10.0f) < D_801543CC)) {
        *temp_a1 = D_801543CC;
        (&D_80110680)[M2C_FIELD(arg0, s8 *, 0x35B)] = 2;
        var_v1 = (&D_80110680)[M2C_FIELD(arg0, s8 *, 0x35B)];
    }
    if (var_v1 == 2) {
        temp_f18 = (&D_80110668)[M2C_FIELD(arg0, s8 *, 0x35B)] * 1000.0f;
        temp_hi = (u32) temp_f18 % 6000U;
        var_f8 = (f32) temp_hi;
        var_f12 = var_f8 * D_801244D8 * D_801244DC;
    } else {
        temp_f8 = D_801543CC * 1000.0f;
        temp_hi_2 = (u32) temp_f8 % 6000U;
        var_f4 = (f32) temp_hi_2;
        var_f12 = var_f4 * D_801244E0 * D_801244E4;
    }
    sp30 = var_f12;
    temp_f6 = cosf(var_f12) * 15.0f;
    vector[1] = 10.0f;
    vector[0] = temp_f6;
    vector[2] = sinf(var_f12) * 15.0f;
    (&D_80152720)[sp2C] = 0.0f;
    func_800E8D50(arg0, arg1, arg2, vector);
}
