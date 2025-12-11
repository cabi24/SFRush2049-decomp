extern ? D_8002D750;
extern f64 D_8002D778;
extern f64 D_8002D780;
extern f64 D_8002D788;
extern f32 D_8002D790;
extern ? D_8002D7A0;
extern f64 D_8002D7C8;
extern f64 D_8002D7D0;
extern f64 D_8002D7D8;
extern f32 D_8002D7E0;
extern f32 D_8002D880;

f32 func_80008730(f32 arg0) {
    f64 temp_fa0;
    f64 temp_fa0_2;
    f64 temp_fa1;
    f64 temp_fv0;
    f64 temp_fv0_2;
    f64 temp_fv1;
    f64 temp_fv1_2;
    f64 temp_fv1_3;
    s32 temp_t6;
    s32 var_v0;

    temp_t6 = ((bitwise s32) arg0 >> 0x16) & 0x1FF;
    if (temp_t6 < 0xFF) {
        temp_fv1_2 = (f64) arg0;
        if (temp_t6 >= 0xE6) {
            temp_fa0_2 = temp_fv1_2 * temp_fv1_2;
            return (f32) ((temp_fv1_2 * temp_fa0_2 * (D_8002D750.unk8 + (((((D_8002D750.unk20 * temp_fa0_2) + D_8002D750.unk18) * temp_fa0_2) + D_8002D750.unk10) * temp_fa0_2))) + temp_fv1_2);
        }
        return arg0;
    }
    if (temp_t6 < 0x136) {
        temp_fv1_3 = (f64) arg0;
        temp_fv0 = temp_fv1_3 * D_8002D778;
        if (temp_fv0 >= 0.0) {
            var_v0 = (s32) (temp_fv0 + 0.5);
        } else {
            var_v0 = (s32) (temp_fv0 - 0.5);
        }
        temp_fv0_2 = (f64) var_v0;
        temp_fv1 = (temp_fv1_3 - (temp_fv0_2 * D_8002D780)) - (temp_fv0_2 * D_8002D788);
        temp_fa0 = temp_fv1 * temp_fv1;
        temp_fa1 = D_8002D750.unk8 + (((((D_8002D750.unk20 * temp_fa0) + D_8002D750.unk18) * temp_fa0) + D_8002D750.unk10) * temp_fa0);
        if (!(var_v0 & 1)) {
            return (f32) ((temp_fv1 * temp_fa0 * temp_fa1) + temp_fv1);
        }
        return -(f32) ((temp_fv1 * temp_fa0 * temp_fa1) + temp_fv1);
    }
    if (arg0 != arg0) {
        return D_8002D880;
    }
    return D_8002D790;
}

f32 func_800088F0(f32 arg0) {
    f32 var_fv0;
    f64 temp_fa0;
    f64 temp_fa1;
    f64 temp_fa1_2;
    f64 temp_ft4;
    f64 temp_fv0;
    f64 temp_fv1;
    s32 var_v0;

    if ((((bitwise s32) arg0 >> 0x16) & 0x1FF) < 0x136) {
        if (arg0 > 0.0f) {
            var_fv0 = arg0;
        } else {
            var_fv0 = -arg0;
        }
        temp_fa0 = (f64) var_fv0;
        temp_fa1_2 = (temp_fa0 * D_8002D7C8) + 0.5;
        if (temp_fa1_2 >= 0.0) {
            var_v0 = (s32) (temp_fa1_2 + 0.5);
        } else {
            var_v0 = (s32) (temp_fa1_2 - 0.5);
        }
        temp_fv0 = (f64) var_v0 - 0.5;
        temp_fv1 = (temp_fa0 - (temp_fv0 * D_8002D7D0)) - (temp_fv0 * D_8002D7D8);
        temp_fa1 = temp_fv1 * temp_fv1;
        temp_ft4 = D_8002D7A0.unk8 + (((((D_8002D7A0.unk20 * temp_fa1) + D_8002D7A0.unk18) * temp_fa1) + D_8002D7A0.unk10) * temp_fa1);
        if (!(var_v0 & 1)) {
            return (f32) ((temp_fv1 * temp_fa1 * temp_ft4) + temp_fv1);
        }
        return -(f32) ((temp_fv1 * temp_fa1 * temp_ft4) + temp_fv1);
    }
    if (arg0 != arg0) {
        return D_8002D880;
    }
    return D_8002D7E0;
}
