f32 func_80008730(f32);                             /* extern */
f32 func_800088F0(f32, f32);                        /* extern */
? func_800091E0(? *, s32);                          /* extern */
? func_80009394(f32);                               /* extern */
extern f64 D_8002D7F0;

void func_80008C20(void *arg0, s16 *arg1, f32 arg2, f32 arg3, f32 arg4, f32 arg5, f32 arg6) {
    f32 sp20;
    f32 sp1C;
    f32 temp_fa0;
    f32 temp_fa0_2;
    f32 temp_fa1;
    f32 temp_ft0;
    f32 temp_ft2;
    f32 temp_ft5;
    f32 temp_fv1;
    f32 temp_fv1_2;
    f32 var_fa0;
    f32 var_fa1;
    f32 var_ft3;
    f32 var_ft4;
    f64 temp_fv0;
    s32 var_v1;
    u32 temp_t8;
    void *temp_v0;
    void *var_v0;

    func_80009394(arg2);
    temp_fa1 = (f32) ((f64) arg2 * D_8002D7F0);
    temp_fa0 = temp_fa1 / 2.0f;
    sp1C = temp_fa0;
    sp20 = func_800088F0(temp_fa0, temp_fa1);
    temp_fv1 = sp20 / func_80008730(temp_fa0);
    temp_ft5 = arg4 + arg5;
    var_v0 = arg0;
    temp_fa0_2 = arg4 - arg5;
    var_v0->unk2C = -1.0f;
    var_v0->unk14 = temp_fv1;
    var_v0->unk28 = (f32) (temp_ft5 / temp_fa0_2);
    var_v0->unk0 = (f32) (temp_fv1 / arg3);
    var_v0->unk3C = 0.0f;
    var_v0->unk38 = (f32) ((2.0f * arg4 * arg5) / temp_fa0_2);
    var_v1 = 1;
    var_ft4 = var_v0->unk4;
    var_ft3 = var_v0->unk0 * arg6;
    var_fa0 = var_v0->unk8;
    var_fa1 = var_v0->unkC;
    if (1 != 4) {
        do {
            temp_ft2 = var_ft4 * arg6;
            var_ft4 = var_v0->unk14;
            temp_ft0 = var_fa0 * arg6;
            var_fa0 = var_v0->unk18;
            var_v1 += 1;
            temp_fv1_2 = var_fa1 * arg6;
            var_fa1 = var_v0->unk1C;
            var_v0->unk0 = var_ft3;
            var_ft3 = var_v0->unk10 * arg6;
            var_v0->unk4 = temp_ft2;
            var_v0->unk8 = temp_ft0;
            var_v0 += 0x10;
            var_v0->unk-4 = temp_fv1_2;
        } while (var_v1 != 4);
    }
    temp_v0 = var_v0 + 0x10;
    temp_v0->unk-10 = var_ft3;
    temp_v0->unk-C = (f32) (var_ft4 * arg6);
    temp_v0->unk-8 = (f32) (var_fa0 * arg6);
    temp_v0->unk-4 = (f32) (var_fa1 * arg6);
    if (arg1 != NULL) {
        temp_fv0 = (f64) temp_ft5;
        if (temp_fv0 <= 2.0) {
            *arg1 = 0xFFFF;
            return;
        }
        temp_t8 = (u32) (131072.0 / temp_fv0);
        *arg1 = (s16) temp_t8;
        if ((temp_t8 & 0xFFFF) <= 0) {
            *arg1 = 1;
        }
    }
}

void func_80008E50(s32 arg0, void *arg2, s16 *arg3, f32 arg4, f32 arg5, f32 arg6) {
    ? sp28;

    func_80008C20(arg2, arg3, (bitwise f32) &sp28, (bitwise f32) arg2, (bitwise f32) arg3, arg4, arg5, arg6);
    func_800091E0(&sp28, arg0);
}
