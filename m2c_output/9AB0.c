? func_800091E0(? *, s32);                          /* extern */
? func_80009394();                                  /* extern */
f32 func_8000E3C0(f32, f32);                        /* extern */

void func_80008EB0(void *arg0, f32 arg1, f32 arg2, f32 arg3, f32 arg4, f32 arg5, f32 arg6, f32 arg7, f32 arg8, f32 arg9) {
    f32 temp_fa0;
    f32 temp_fa1;
    f32 temp_fa1_2;
    f32 temp_fs0;
    f32 temp_fs1;
    f32 temp_fs1_2;
    f32 temp_fs2;
    f32 temp_fs2_2;
    f32 temp_fs3;
    f32 temp_fs3_2;
    f32 temp_fs4;
    f32 temp_fs4_2;
    f32 temp_fs5;
    f32 temp_fs5_2;
    f32 temp_ft4;
    f32 temp_ft5;
    f32 temp_fv0;
    f32 temp_fv1;
    f32 temp_fv1_2;
    f32 temp_fv1_3;

    func_80009394();
    temp_fv0 = arg4 - arg1;
    temp_fs1 = arg5 - arg2;
    temp_fs2 = arg6 - arg3;
    temp_fv1 = (f32) (-1.0 / (f64) func_8000E3C0((temp_fv0 * temp_fv0) + (temp_fs1 * temp_fs1) + (temp_fs2 * temp_fs2)));
    temp_fs0 = temp_fv0 * temp_fv1;
    temp_fs1_2 = temp_fs1 * temp_fv1;
    temp_fs2_2 = temp_fs2 * temp_fv1;
    temp_fs3 = (arg8 * temp_fs2_2) - (arg9 * temp_fs1_2);
    temp_fs4 = (arg9 * temp_fs0) - (arg7 * temp_fs2_2);
    temp_fs5 = (arg7 * temp_fs1_2) - (arg8 * temp_fs0);
    temp_fv1_2 = (f32) (1.0 / (f64) func_8000E3C0((temp_fs3 * temp_fs3) + (temp_fs4 * temp_fs4) + (temp_fs5 * temp_fs5)));
    temp_fs3_2 = temp_fs3 * temp_fv1_2;
    temp_fs4_2 = temp_fs4 * temp_fv1_2;
    temp_fs5_2 = temp_fs5 * temp_fv1_2;
    temp_fa1 = (temp_fs1_2 * temp_fs5_2) - (temp_fs2_2 * temp_fs4_2);
    arg7 = temp_fa1;
    temp_ft4 = (temp_fs2_2 * temp_fs3_2) - (temp_fs0 * temp_fs5_2);
    arg8 = temp_ft4;
    temp_ft5 = (temp_fs0 * temp_fs4_2) - (temp_fs1_2 * temp_fs3_2);
    arg9 = temp_ft5;
    temp_fv1_3 = (f32) (1.0 / (f64) func_8000E3C0((temp_fa1 * temp_fa1) + (temp_ft4 * temp_ft4) + (temp_ft5 * temp_ft5), temp_fa1));
    temp_fa1_2 = arg7 * temp_fv1_3;
    temp_fa0 = arg8 * temp_fv1_3;
    arg9 *= temp_fv1_3;
    arg0->unk0 = temp_fs3_2;
    arg0->unk10 = temp_fs4_2;
    arg0->unk20 = temp_fs5_2;
    arg0->unk4 = temp_fa1_2;
    arg0->unk14 = temp_fa0;
    arg0->unk30 = (f32) -((arg1 * temp_fs3_2) + (arg2 * temp_fs4_2) + (arg3 * temp_fs5_2));
    arg8 = temp_fa0;
    arg7 = temp_fa1_2;
    arg0->unk24 = arg9;
    arg0->unk8 = temp_fs0;
    arg0->unk18 = temp_fs1_2;
    arg0->unk28 = temp_fs2_2;
    arg0->unkC = 0.0f;
    arg0->unk1C = 0.0f;
    arg0->unk2C = 0.0f;
    arg0->unk34 = (f32) -((arg1 * arg7) + (arg2 * arg8) + (arg3 * arg9));
    arg0->unk3C = 1.0f;
    arg0->unk38 = (f32) -((arg1 * temp_fs0) + (arg2 * temp_fs1_2) + (arg3 * temp_fs2_2));
}

void func_80009168(s32 arg0, void *arg1, f32 arg2, ? arg3, f32 arg4, f32 arg5, f32 arg6, f32 arg7, f32 arg8, f32 arg9) {
    ? sp30;

    func_80008EB0(arg1, arg2, (bitwise f32) &sp30, (bitwise f32) arg1, arg2, arg4, arg5, arg6, arg7, arg8, arg9);
    func_800091E0(&sp30, arg0);
}
