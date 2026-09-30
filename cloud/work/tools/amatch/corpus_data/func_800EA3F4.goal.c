void func_800EA3F4(void *arg0, void *arg1) {
    s32 idx;
    f32 len;
    f32 sp94;
    f32 sp90;
    s32 i;
    s32 unused;
    f32 sp84;
    f32 f2;
    f32 sp74[3];
    f32 sp68[3];
    f32 sp5C[3];
    f32 sp50[3];
    f32 sp44[3];
    f32 f12;

    idx = CB(0x35C);
    sp94 = D_801526F8[idx];
    sp90 = D_801526E0[idx];
    func_800E8CB8(arg0, arg1, car + 0x50);
    if (CB(0x35D) == 3) {
        sp94 -= 6.0f;
        sp90 -= 2.0f;
    }
    len = func_8008B3C8((f32 *)(car + 0x14));
    if (len > 100.0f) {
        f2 = 0.25f;
    } else {
        f2 = len * D_801244EC;
    }
    sp84 = sqrtf(sp94 * sp94 + sp90 * sp90) * (1.0f + f2);
    sp94 *= 1.0f + f2;
    if (D_80152574[idx] != 0 || D_8014A110 == 6) {
        f12 = 0.0;
    } else if (len < 140.0f) {
        f12 = D_801244F4 - (len / 140.0f) * D_801244F0;
    } else {
        f12 = D_801244F8;
    }
    D_80152574[idx] = 0;
    if (D_801525F8[idx] >= 3 && D_80152680[idx] > 0.5f) {
        D_801551D8[idx] = D_801551D8[idx] + D_8002EB94;
    } else if (len < 30.0f) {
        D_801551D8[idx] += D_8002EB94 * 10.0f;
    } else {
        D_801551D8[idx] = D_801551D8[idx] - D_8002EB94;
    }
    if (D_801551D8[idx] < 0.0f) {
        D_801551D8[idx] = 0.0f;
    } else if (D_801551D8[idx] > 1.0f) {
        D_801551D8[idx] = 1.0f;
    }
    if (D_801525F8[idx] < 3) {
        D_80155210[idx] -= D_8002EB94;
    } else {
        if (D_80152680[idx] > 1.5f) {
            D_80155210[idx] += D_8002EB94 * 10.0f;
        } else if (D_80152680[idx] > 0.5f) {
            D_80155210[idx] += D_8002EB94 / (D_80124500 + (1.5f - D_80152680[idx]) * D_801244FC);
        } else {
            D_80155210[idx] += D_8002EB94;
        }
    }
    if (D_80155210[idx] < 0.0f) {
        D_80155210[idx] = 0.0f;
    } else if (D_80155210[idx] > 1.0f) {
        D_80155210[idx] = 1.0f;
    }
    for (i = 0; i < 3; i++) {
        sp74[i] = -CF(0x44 + i * 4);
    }
    if (D_801551D8[idx] < 1.0f) {
        if (len < 1.0f) {
            sp68[0] = sp74[0];
            sp68[1] = sp74[1];
            sp68[2] = sp74[2];
        } else if (CF(0xAC) < 0 && D_801525F8[idx] >= 3) {
            sp68[0] = CF(0x14) * (1 / len);
            sp68[1] = CF(0x18) * (1 / len);
            sp68[2] = CF(0x1C) * (1 / len);
        } else {
            sp68[0] = CF(0x14) * (-1 / len);
            sp68[1] = CF(0x18) * (-1 / len);
            sp68[2] = CF(0x1C) * (-1 / len);
        }
    }
    if (D_801551D8[idx] == 0.0f) {
        sp5C[0] = sp68[0]; sp5C[1] = sp68[1]; sp5C[2] = sp68[2];
    } else if (D_801551D8[idx] == 1.0f) {
        sp5C[0] = sp74[0]; sp5C[1] = sp74[1]; sp5C[2] = sp74[2];
    } else {
        func_800CFDEC(sp68, sp74, 3, 0.0f, 1.0f, D_801551D8[idx], sp5C);
        len = func_8008B3C8(sp5C);
        if (len < D_80124504) {
            sp5C[0] = sp74[0]; sp5C[1] = sp74[1]; sp5C[2] = sp74[2];
        } else {
            sp5C[0] *= (1.0f / len); sp5C[1] *= (1.0f / len); sp5C[2] *= (1.0f / len);
        }
    }
    if (f12 > 0) {
        sp5C[0] *= (1.0f - f12); sp5C[1] *= (1.0f - f12); sp5C[2] *= (1.0f - f12);
        sp5C[0] += D_80155178[idx][0] * f12;
        sp5C[1] += D_80155178[idx][1] * f12;
        sp5C[2] += D_80155178[idx][2] * f12;
        len = func_8008B3C8(sp5C);
        if (len < D_80124508) {
            sp5C[0] = sp74[0]; sp5C[1] = sp74[1]; sp5C[2] = sp74[2];
        } else {
            sp5C[0] *= (1.0f / len); sp5C[1] *= (1.0f / len); sp5C[2] *= (1.0f / len);
        }
    }
    D_80155178[idx][0] = sp5C[0];
    D_80155178[idx][1] = sp5C[1];
    D_80155178[idx][2] = sp5C[2];
    for (i = 0; i < 3; i++) {
        sp74[i] = CF(0x38 + i * 4);
    }
    if (D_80155210[idx] < 1.0f) {
        sp68[0] = D_801141BC[0];
        sp68[1] = D_801141BC[1];
        sp68[2] = D_801141BC[2];
    }
    if (D_80155210[idx] == 0) {
        sp50[0] = sp68[0]; sp50[1] = sp68[1]; sp50[2] = sp68[2];
    } else if (D_80155210[idx] == 1.0f) {
        sp50[0] = sp74[0]; sp50[1] = sp74[1]; sp50[2] = sp74[2];
    } else {
        func_800CFDEC(sp68, sp74, 3, 0, 1.0f, D_80155210[idx], sp50);
        len = func_8008B3C8(sp50);
        if (len < D_8012450C) {
            sp50[0] = sp74[0]; sp50[1] = sp74[1]; sp50[2] = sp74[2];
        } else {
            sp50[0] *= (1.0f / len); sp50[1] *= (1.0f / len); sp50[2] *= (1.0f / len);
        }
    }
    if (f12 > 0) {
        sp50[0] *= (1.0f - f12); sp50[1] *= (1.0f - f12); sp50[2] *= (1.0f - f12);
        sp50[0] += D_801551A8[idx][0] * f12;
        sp50[1] += D_801551A8[idx][1] * f12;
        sp50[2] += D_801551A8[idx][2] * f12;
        len = func_8008B3C8(sp50);
        if (len < D_80124510) {
            sp50[0] = sp74[0]; sp50[1] = sp74[1]; sp50[2] = sp74[2];
        } else {
            sp50[0] *= (1.0f / len); sp50[1] *= (1.0f / len); sp50[2] *= (1.0f / len);
        }
    }
    D_801551A8[idx][0] = sp50[0];
    D_801551A8[idx][1] = sp50[1];
    D_801551A8[idx][2] = sp50[2];
    sp44[0] = sp5C[0] * sp94; sp44[1] = sp5C[1] * sp94; sp44[2] = sp5C[2] * sp94;
    sp44[0] += sp50[0] * sp90;
    sp44[1] += sp50[1] * sp90;
    sp44[2] += sp50[2] * sp90;
    vector_copy_scale(sp44, D_80150B70[idx] + 0x78);
    sp44[0] = OF(0x78) * sp84;
    sp44[1] = OF(0x7C) * sp84;
    sp44[2] = OF(0x80) * sp84;
    if (CB(0x35D) == 2) {
        sp44[0] += sp50[0] * 4.0f;
        sp44[1] += sp50[1] * 4.0f;
        sp44[2] += sp50[2] * 4.0f;
    } else if (CB(0x35D) == 3) {
        sp44[0] += sp50[0] * 4.0f;
        sp44[1] += sp50[1] * 4.0f;
        sp44[2] += sp50[2] * 4.0f;
    }
    OF(0x84) = A1(0) + sp44[0];
    OF(0x88) = A1(4) + sp44[1];
    OF(0x8C) = A1(8) + sp44[2];
    OF(0x78) *= -1.0f;
    OF(0x7C) *= -1.0f;
    OF(0x80) *= -1.0f;
    OF(0x60) = sp50[1] * OF(0x80) - sp50[2] * OF(0x7C);
    OF(0x64) = sp50[2] * OF(0x78) - sp50[0] * OF(0x80);
    OF(0x68) = sp50[0] * OF(0x7C) - sp50[1] * OF(0x78);
    len = func_8008B3C8((f32 *)(D_80150B70[idx] + 0x60));
    if (len < D_80124514) {
        OF(0x60) = D_801141C8[0];
        OF(0x64) = D_801141C8[1];
        OF(0x68) = D_801141C8[2];
    } else {
        OF(0x60) *= (1.0f / len);
        OF(0x64) *= (1.0f / len);
        OF(0x68) *= (1.0f / len);
    }
    OF(0x6C) = OF(0x7C) * OF(0x68) - OF(0x80) * OF(0x64);
    OF(0x70) = OF(0x80) * OF(0x60) - OF(0x78) * OF(0x68);
    OF(0x74) = OF(0x78) * OF(0x64) - OF(0x7C) * OF(0x60);
}