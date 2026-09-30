void func_800E627C(void *arg0, void *arg1) {
    f32 temp_f12;
    f32 temp_f12_2;
    f32 temp_f2;
    f32 temp_f2_2;
    f32 var_f0;
    f32 var_f12;
    f32 var_f14;
    f32 var_f14_2;
    f32 var_f2;

    if ((s8) D_8013FECB != 0) {
        M2C_FIELD(arg0, f32 *, 0x720) = 0.0f;
        return;
    }
    temp_f2 = *((f32 *) ((u8 *) &D_80140620 + (M2C_FIELD(arg1, u8 *, 1) * 8)));
    var_f0 = M2C_FIELD(arg0, f32 *, 0x720);
    if (M2C_FIELD(arg1, s32 *, 0x20) == 0x19) {
        if (temp_f2 < D_80124498) {
            var_f2 = temp_f2 + D_8012449C;
        } else if (D_801244A0 < temp_f2) {
            var_f2 = temp_f2 - D_801244A0;
        } else {
            var_f2 = 0.0f;
        }
    } else {
        if (temp_f2 >= 0.0f) {
            var_f12 = temp_f2;
        } else {
            var_f12 = -temp_f2;
        }
        if (temp_f2 >= 0.0f) {
            var_f14 = temp_f2;
        } else {
            var_f14 = -temp_f2;
        }
        var_f2 = var_f14 * var_f12 * temp_f2;
    }
    temp_f12 = var_f2 * 127.0f;
    if (temp_f12 < 0.0f) {
        var_f14_2 = temp_f12 - 0.5f;
    } else {
        var_f14_2 = temp_f12 + 0.5f;
    }
    temp_f2_2 = (f32) (s32) var_f14_2 / 127.0f;
    temp_f12_2 = temp_f2_2 - var_f0;
    if (D_801244A4 < temp_f12_2) {
        goto block_21;
    }
    if (temp_f12_2 < D_801244A8) {
block_21:
        var_f0 = temp_f2_2;
    }
    if ((((s8) D_80151AD8 != 0) || (D_80140A04 != 0)) && (((s8) D_80151AD8 == 0) || (D_80140A04 == 0) || (D_8013F1D9 != 0))) {
        var_f0 = -var_f0;
    }
    M2C_FIELD(arg0, f32 *, 0x720) = var_f0;
}