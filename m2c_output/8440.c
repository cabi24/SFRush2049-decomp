? func_80007CA0(? *, ?);                            /* extern */
? func_8000D2B0(? *, ?);                            /* extern */
s32 func_8000D5C0(s32);                             /* extern */
? func_8000D640(?);                                 /* extern */
s32 func_8000D650(?);                               /* extern */
s32 func_8000D680(?, ?, ? *, s32);                  /* extern */
s32 func_8000D710();                                /* extern */
extern ? D_80036790;
extern s32 D_800367A0;
extern s32 D_800367A8;
extern s32 D_800367B0;
extern s32 D_800367B8;
extern s32 D_800367BC;
extern s32 D_800367C0;
extern s32 D_800367C8;
extern ? D_A0000000;

? *func_80007840(void) {
    func_8000D2B0(&D_80036790, 0x40);
    if (D_800367A0 != 0) {
        D_800367A0 = func_8000D5C0(D_800367A0);
    }
    if (D_800367A8 != 0) {
        D_800367A8 = func_8000D5C0(D_800367A8);
    }
    if (D_800367B0 != 0) {
        D_800367B0 = func_8000D5C0(D_800367B0);
    }
    if (D_800367B8 != 0) {
        D_800367B8 = func_8000D5C0(D_800367B8);
    }
    if (D_800367BC != 0) {
        D_800367BC = func_8000D5C0(D_800367BC);
    }
    if (D_800367C0 != 0) {
        D_800367C0 = func_8000D5C0(D_800367C0);
    }
    if (D_800367C8 != 0) {
        D_800367C8 = func_8000D5C0(D_800367C8);
    }
    return &D_80036790;
}

void func_8000794C(void *arg0) {
    ? *temp_v0;

    temp_v0 = func_80007840();
    if (temp_v0->unk4 & 1) {
        temp_v0->unk18 = (s32) temp_v0->unk38;
        temp_v0->unk1C = (s32) temp_v0->unk3C;
        arg0->unk4 = (s32) (arg0->unk4 & ~1);
        if (temp_v0->unk4 & 4) {
            temp_v0->unk10 = (s32) *((arg0->unk38 + 0xBFC) | (s32) &D_A0000000);
        }
    }
    func_80007CA0(temp_v0, 0x40);
    func_8000D640(0x2B00);
    if (func_8000D650(0x04001000) == -1) {
        do {

        } while (func_8000D650(0x04001000) == -1);
    }
    if (func_8000D680(1, 0x04000FC0, temp_v0, 0x40) == -1) {
        do {

        } while (func_8000D680(1, 0x04000FC0, temp_v0, 0x40) == -1);
    }
    if (func_8000D710() != 0) {
        do {

        } while (func_8000D710() != 0);
    }
    if (func_8000D680(1, 0x04001000, temp_v0->unk8, temp_v0->unkC) == -1) {
        do {

        } while (func_8000D680(1, 0x04001000, temp_v0->unk8, temp_v0->unkC) == -1);
    }
}

void func_80007AB4(s32 arg0) {
    if (func_8000D710() != 0) {
        do {

        } while (func_8000D710() != 0);
    }
    func_8000D640(0x125);
}
