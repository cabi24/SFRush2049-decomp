extern s64 D_8002D800;
extern s64 D_8002D808;

s64 func_800099F0(void) {
    M2C_ERROR(/* unknown instruction: trunc.l.d $f4, $f12 */);
    return (s64) M2C_ERROR(/* unknown instruction: dmfc1 $v0, $f4 */) >> 0x20;
}

s64 func_80009A0C(void) {
    M2C_ERROR(/* unknown instruction: trunc.l.s $f4, $f12 */);
    return (s64) M2C_ERROR(/* unknown instruction: dmfc1 $v0, $f4 */) >> 0x20;
}

s64 func_80009A28(f64 arg0) {
    s64 var_v0;

    M2C_ERROR(/* unknown instruction: cvt.l.d $f4, $f12 */);
    if (M2C_ERROR(/* cfc1 */) & 0x78) {
        M2C_ERROR(/* unknown instruction: cvt.l.d $f4, $f4 */);
        if (!(M2C_ERROR(/* cfc1 */) & 0x78)) {
            var_v0 = M2C_ERROR(/* unknown instruction: dmfc1 $v0, $f4 */) | D_8002D800;
        } else {
            goto block_3;
        }
    } else {
        var_v0 = M2C_ERROR(/* unknown instruction: dmfc1 $v0, $f4 */);
        if (var_v0 < 0) {
block_3:
            var_v0 = -1;
        }
    }
    return var_v0 >> 0x20;
}

s64 func_80009AC8(f32 arg0) {
    s64 var_v0;

    M2C_ERROR(/* unknown instruction: cvt.l.s $f4, $f12 */);
    if (M2C_ERROR(/* cfc1 */) & 0x78) {
        M2C_ERROR(/* unknown instruction: cvt.l.s $f4, $f4 */);
        if (!(M2C_ERROR(/* cfc1 */) & 0x78)) {
            var_v0 = M2C_ERROR(/* unknown instruction: dmfc1 $v0, $f4 */) | D_8002D808;
        } else {
            goto block_3;
        }
    } else {
        var_v0 = M2C_ERROR(/* unknown instruction: dmfc1 $v0, $f4 */);
        if (var_v0 < 0) {
block_3:
            var_v0 = -1;
        }
    }
    return var_v0 >> 0x20;
}

f32 func_80009B64(s64 arg0, ? arg1) {
    M2C_ERROR(/* unknown instruction: dmtc1 $t6, $f4 */);
    return M2C_ERROR(/* unknown instruction: cvt.d.l $f0, $f4 */);
}

f32 func_80009B7C(s64 arg0, ? arg1) {
    M2C_ERROR(/* unknown instruction: dmtc1 $t6, $f4 */);
    return M2C_ERROR(/* unknown instruction: cvt.s.l $f0, $f4 */);
}

f64 func_80009B94(s64 arg0, ? arg1) {
    f64 var_fv0;

    var_fv0 = M2C_ERROR(/* unknown instruction: cvt.d.l $f0, $f4 */);
    if (M2C_ERROR(/* unknown instruction: dmtc1 $t6, $f4 */) < 0) {
        var_fv0 += 4294967296.0;
    }
    return var_fv0;
}

f32 func_80009BC8(s64 arg0, ? arg1) {
    f32 var_fv0;

    var_fv0 = M2C_ERROR(/* unknown instruction: cvt.s.l $f0, $f4 */);
    if (M2C_ERROR(/* unknown instruction: dmtc1 $t6, $f4 */) < 0) {
        var_fv0 += 4294967296.0f;
    }
    return var_fv0;
}
