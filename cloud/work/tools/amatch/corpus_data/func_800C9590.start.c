f32 func_800C9590(f32 arg1, s32 arg0) {
    f32 temp_f0;
    f32 temp_f16;
    f32 var_f2;

    temp_f16 = -arg0;
    temp_f0 = ((f32) arg0 * arg0) / arg1;
    var_f2 = temp_f0;
    if (temp_f0 < temp_f16) {
        return temp_f16;
    }
    if (arg0 < temp_f0) {
        var_f2 = arg0;
    }
    return var_f2;
}