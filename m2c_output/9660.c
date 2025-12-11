? func_800091E0(? *, s32);                          /* extern */
? func_80009394();                                  /* extern */

void func_80008A60(void *arg0, f32 arg1, f32 arg2, f32 arg3, f32 arg4, f32 arg5, f32 arg6, f32 arg7) {
    f32 temp_fa0;
    f32 temp_ft0;
    f32 temp_ft1;
    f32 temp_ft3;
    f32 temp_fv0;
    f32 temp_fv1;
    f32 var_fa0;
    f32 var_fa1;
    f32 var_ft4;
    f32 var_ft5;
    s32 var_v1;
    void *temp_v0;
    void *var_v0;

    func_80009394();
    temp_fv0 = arg2 - arg1;
    var_v0 = arg0;
    temp_fa0 = arg6 - arg5;
    var_v0->unk0 = (f32) (2.0f / temp_fv0);
    temp_fv1 = arg4 - arg3;
    var_v0->unk28 = (f32) (-2.0f / temp_fa0);
    var_v0->unk14 = (f32) (2.0f / temp_fv1);
    var_v0->unk30 = (f32) (-(arg2 + arg1) / temp_fv0);
    var_v0->unk34 = (f32) (-(arg4 + arg3) / temp_fv1);
    var_v0->unk3C = 1.0f;
    var_v0->unk38 = (f32) (-(arg6 + arg5) / temp_fa0);
    var_v1 = 1;
    var_ft5 = var_v0->unk4;
    var_fa0 = var_v0->unk0 * arg7;
    var_fa1 = var_v0->unk8;
    var_ft4 = var_v0->unkC;
    if (1 != 4) {
        do {
            temp_ft3 = var_ft5 * arg7;
            var_ft5 = var_v0->unk14;
            temp_ft1 = var_fa1 * arg7;
            var_fa1 = var_v0->unk18;
            var_v1 += 1;
            temp_ft0 = var_ft4 * arg7;
            var_ft4 = var_v0->unk1C;
            var_v0->unk0 = var_fa0;
            var_fa0 = var_v0->unk10 * arg7;
            var_v0->unk4 = temp_ft3;
            var_v0->unk8 = temp_ft1;
            var_v0 += 0x10;
            var_v0->unk-4 = temp_ft0;
        } while (var_v1 != 4);
    }
    temp_v0 = var_v0 + 0x10;
    temp_v0->unk-10 = var_fa0;
    temp_v0->unk-C = (f32) (var_ft5 * arg7);
    temp_v0->unk-8 = (f32) (var_fa1 * arg7);
    temp_v0->unk-4 = (f32) (var_ft4 * arg7);
}

void func_80008BB4(s32 arg0, void *arg1, f32 arg2, ? arg3, f32 arg4, f32 arg5, f32 arg6, f32 arg7) {
    ? sp28;

    func_80008A60(arg1, arg2, (bitwise f32) &sp28, (bitwise f32) arg1, arg2, arg4, arg5, arg6, arg7);
    func_800091E0(&sp28, arg0);
}
