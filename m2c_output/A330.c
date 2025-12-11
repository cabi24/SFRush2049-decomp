? func_80007270(s32, ?, ?);                         /* extern */
? func_800095AC(? *, s32);                          /* extern */
? func_80009658(?);                                 /* extern */
s32 func_8000E4B0(?, s8 *);                         /* extern */
? func_8000E5B0();                                  /* extern */
? func_8000E5F4();                                  /* extern */
void func_800098E0();                               /* static */
extern s8 D_80037AA0;
extern ? D_80037ADC;
extern u8 D_80037AE0;
extern u8 D_80037AE1;

s32 func_80009730(s32 arg0) {
    s32 sp1C;
    s32 temp_v0;

    func_8000E5B0();
    if (D_80037AE0 != 0) {
        func_80009658(0);
        func_8000E4B0(1, &D_80037AA0);
        func_80007270(arg0, 0, 1);
    }
    temp_v0 = func_8000E4B0(0, &D_80037AA0);
    sp1C = temp_v0;
    D_80037AE0 = 0;
    func_8000E5F4();
    return temp_v0;
}

void func_800097AC(s32 arg0) {
    ? sp1F;

    func_800095AC(&sp1F, arg0);
}

s32 func_800097D0(s32 arg0) {
    s32 sp1C;
    s32 temp_v0;

    func_8000E5B0();
    if (D_80037AE0 != 1) {
        func_800098E0();
        func_8000E4B0(1, &D_80037AA0);
        func_80007270(arg0, 0, 1);
    }
    temp_v0 = func_8000E4B0(0, &D_80037AA0);
    sp1C = temp_v0;
    D_80037AE0 = 1;
    func_8000E5F4();
    return temp_v0;
}

void func_80009854(void *arg0) {
    ? sp4;
    s32 temp_t1;
    s32 var_v1;
    s8 *var_v0;
    void *var_a0;

    var_a0 = arg0;
    var_v0 = &D_80037AA0;
    var_v1 = 0;
    if ((s32) D_80037AE1 > 0) {
        do {
            sp4.unk0 = (s32) (unaligned s32) var_v0->unk0;
            sp4.unk4 = (s32) (unaligned s32) var_v0->unk4;
            temp_t1 = (s32) (sp6 & 0xC0) >> 4;
            var_a0->unk4 = (s8) temp_t1;
            if (!(temp_t1 & 0xFF)) {
                var_a0->unk0 = sp8;
                var_a0->unk2 = spA;
                var_a0->unk3 = spB;
            }
            var_v1 += 1;
            var_v0 += 8;
            var_a0 += 6;
        } while (var_v1 < (s32) D_80037AE1);
    }
}

void func_800098E0(void) {
    s8 sp13;
    s8 sp12;
    s16 sp10;
    s8 spF;
    s8 spE;
    s8 spD;
    s8 spC;
    s32 var_v1;
    s8 *var_a0;
    s8 *var_v0;

    var_v0 = &D_80037AA0;
    var_a0 = &D_80037AA0;
    do {
        var_a0 += 4;
        var_a0->unk-4 = 0;
    } while ((u32) var_a0 < (u32) &D_80037ADC);
    D_80037AA0.unk3C = 1;
    spC = 0xFF;
    spD = 1;
    spE = 4;
    spF = 1;
    sp10 = 0xFFFF;
    sp12 = -1;
    sp13 = -1;
    var_v1 = 0;
    if ((s32) D_80037AE1 > 0) {
        do {
            var_v1 += 1;
            var_v0 += 8;
            var_v0->unk-8 = (unaligned s32) spC.unk0;
            var_v0->unk-4 = (unaligned s32) spC.unk4;
        } while (var_v1 < (s32) D_80037AE1);
    }
    *var_v0 = 0xFE;
}
