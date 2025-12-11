? func_80006D40(? *);                               /* extern */
? func_80006DA0(?);                                 /* extern */
s32 func_800071D0(s32);                             /* extern */
? func_80007C68(? *, ? *, ?);                       /* extern */
? func_80007CA0(? *, ?);                            /* extern */
? func_80007D20(?);                                 /* extern */
? func_800A7508(f32, f32);                          /* extern */
extern f32 D_8002AFB4;
extern f32 D_8002AFB8;
extern ? D_8002AFC8;
extern ? D_8002AFF8;
extern ? D_8002AFFA;
extern ? D_8002AFFC;
extern ? D_8002AFFE;
extern s16 D_8002B010;
extern ? *D_8002B014;
extern ? D_8002B1B0;
extern f32 D_8002D4C0;
extern f32 D_8002D4C4;
extern f32 D_8002D4C8;
extern f32 D_8002D4CC;
extern s16 D_8002EBF8;
extern s16 D_8002EBFA;
extern ? *D_8002EBFC;
extern ? D_8002EC00;
extern s16 D_8002ECA0;
extern s16 D_8002ECA2;
extern s16 D_8002ECA4;
extern s16 D_8002ECA6;
extern s16 D_8002ECA8;
extern s16 D_8002ECAA;
extern s16 D_8002ECAC;
extern s16 D_8002ECAE;
extern s16 D_8002ECB0;
extern s16 D_8002ECB2;
extern s32 osTvType;

void func_800015F0(void) {
    s32 sp34;
    s32 sp30;
    s32 sp2C;
    s32 sp28;
    s32 sp24;
    u32 sp20;
    f32 sp1C;
    f32 sp18;
    s32 temp_t2;

    if (D_8002EBFC == NULL) {
        return;
    }
    sp30 = D_8002ECA0 + D_8002ECB0;
    sp28 = D_8002ECA4 + D_8002ECB0 + D_8002ECA8;
    sp2C = D_8002ECA2 + D_8002ECB2;
    temp_t2 = D_8002ECA6 + D_8002ECB2 + D_8002ECAA;
    sp24 = temp_t2;
    sp1C = (f32) (D_8002ECA4 - D_8002ECA0) / (f32) (sp28 - sp30);
    sp18 = (f32) (D_8002ECA6 - D_8002ECA2) / (f32) (temp_t2 - sp2C);
    sp34 = func_800071D0(1);
    if (&D_8002EC00 == D_8002B014) {
        D_8002B014 = &D_8002EC00 + 0x50;
    } else {
        D_8002B014 = &D_8002EC00;
    }
    func_80007C68(D_8002B014, D_8002EBFC, 0x50);
    sp30 = (s32) ((f32) sp30 - ((1.0f - ((f32) D_8002B014->unk20 / (f32) D_8002EBFC->unk20)) * 5.0f));
    D_8002B014->unk1C = (s32) ((sp30 << 0x10) | sp28);
    D_8002B014->unk20 = (u32) ((f32) D_8002ECAC * sp1C * D_8002D4C0);
    D_8002B014->unk30 = (s32) (((sp2C & 0x3FF) << 0x10) | (sp24 & 0x3FF));
    D_8002B014->unk28 = (s32) (D_8002ECAC * 2);
    sp20 = (u32) (((f32) D_8002ECAE * sp18 * D_8002D4C4) + 16.5f);
    if (D_8002EBFC->unk3C != D_8002EBFC->unk28) {
        D_8002B014->unk3C = (s32) (D_8002ECAC * 4);
        D_8002B014->unk8 = (s32) (D_8002ECAC * 2);
        sp20 = sp20 >> 1;
    } else {
        D_8002B014->unk3C = (s32) (D_8002ECAC * 2);
        D_8002B014->unk8 = (s32) D_8002ECAC;
    }
    if (D_8002B014->unk4 & 0x40) {
        D_8002B014->unk44 = (s32) ((((sp2C + 2) & 0x3FF) << 0x10) | ((sp24 + 2) & 0x3FF));
    } else {
        D_8002B014->unk44 = (s32) (((sp2C & 0x3FF) << 0x10) | (sp24 & 0x3FF));
    }
    sp20 &= 0xFFF0;
    D_8002B014->unk2C = (s32) ((D_8002B014->unk2C & 0xFFFF0000) | sp20);
    D_8002B014->unk40 = (s32) ((D_8002B014->unk40 & 0xFFFF0000) | sp20);
    D_8002B010 = 2;
    func_80007CA0(D_8002B014, 0x50);
    func_800071D0(sp34);
}

void func_80001B44(s32 arg0, s32 arg1, s32 arg2, s32 arg3) {
    if (arg0 == 0) {
        D_8002AFB4 = 50.0f;
        D_8002AFB8 = D_8002D4C8;
    } else {
        D_8002AFB4 = 60.0f;
        D_8002AFB8 = D_8002D4CC;
    }
    D_8002EBF8 = (s16) arg0;
    D_8002EBFA = (s16) arg1;
    D_8002EBFC = (arg1 * 0x50) + &D_8002B1B0;
    D_8002ECA0 = ((u32) D_8002EBFC->unk1C >> 0x10) & 0x3FF;
    D_8002ECA4 = D_8002EBFC->unk1C & 0x3FF;
    D_8002ECA2 = ((u32) D_8002EBFC->unk30 >> 0x10) & 0x3FF;
    D_8002ECA6 = D_8002EBFC->unk30 & 0x3FF;
    if (D_8002B014 == NULL) {
        D_8002ECA8 = *(&D_8002AFF8 + (D_8002EBF8 * 8));
        D_8002ECAA = *(&D_8002AFFA + (D_8002EBF8 * 8));
        D_8002ECB0 = *(&D_8002AFFC + (D_8002EBF8 * 8));
        D_8002ECB2 = *(&D_8002AFFE + (D_8002EBF8 * 8));
    } else {
        func_800A7508((f32) arg2 / (f32) D_8002ECAC, (f32) arg3 / (f32) D_8002ECAE);
    }
    D_8002ECAC = (s16) arg2;
    D_8002ECAE = (s16) arg3;
    func_800015F0();
}

void func_80001D60(void) {
    s16 temp_t8;

    if (D_8002B010 == 0) {
        return;
    }
    temp_t8 = D_8002B010 - 1;
    D_8002B010 = temp_t8;
    if (temp_t8 == 0) {
        if ((D_8002EBFC->unk2C & 0xFFFF) == (D_8002B014->unk2C & 0xFFFF)) {
            func_80006DA0(0);
        }
        func_80006D40(D_8002B014);
        func_80007D20(0xAA);
    }
}

s32 func_80001DFC(void) {
    s32 sp4;

    if (osTvType == 1) {
        sp4 = 0;
    } else if (osTvType == 0) {
        sp4 = 0xE;
    } else {
        sp4 = 0x1C;
    }
    return sp4;
}

void func_80001E58(void) {
    func_80006D40(D_8002EBFC);
}

void func_80001E84(s32 *arg0, s32 *arg1) {
    *arg0 = (s32) *(&D_8002AFFC + (D_8002EBF8 * 8));
    *arg1 = (s32) *(&D_8002AFFE + (D_8002EBF8 * 8));
}

void func_80001ECC(s32 *arg0, s32 *arg1) {
    *arg0 = D_8002ECB0 - *(&D_8002AFFC + (D_8002EBF8 * 8));
    *arg1 = D_8002ECB2 - *(&D_8002AFFE + (D_8002EBF8 * 8));
}

void func_80001F2C(s32 arg0, s32 arg1) {
    s32 sp24;
    s32 sp20;
    s32 sp1C;
    s32 sp18;
    void *temp_t2;
    void *temp_t7;

    if (D_8002B014 == NULL) {
        return;
    }
    sp24 = D_8002ECA0 + arg0;
    sp20 = D_8002ECA2 + arg1;
    sp1C = D_8002ECA4 + arg0 + D_8002ECA8;
    sp18 = D_8002ECA6 + arg1 + D_8002ECAA;
    temp_t7 = (D_8002EBF8 * 0x10) + &D_8002AFC8;
    if ((temp_t7->unk0 < sp24) && (sp24 < temp_t7->unk2) && (temp_t7->unk4 < sp1C) && (sp1C < temp_t7->unk6)) {
        D_8002ECB0 = *(&D_8002AFFC + (D_8002EBF8 * 8)) + arg0;
    }
    temp_t2 = (D_8002EBF8 * 0x10) + &D_8002AFC8;
    if ((temp_t2->unk8 < sp20) && (sp20 < temp_t2->unkA) && (temp_t2->unkC < sp18) && (sp18 < temp_t2->unkE)) {
        D_8002ECB2 = *(&D_8002AFFE + (D_8002EBF8 * 8)) + arg1;
    }
    func_800015F0();
}
