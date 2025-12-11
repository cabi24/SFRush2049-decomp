? func_80006A00(? *, ? *, ?);                       /* extern */
? func_80007270(? *, ? *, ?);                       /* extern */
s32 func_800073B0();                                /* extern */
? func_8000E3D0(? *, s32, s32, ? *, ? *);           /* extern */
s32 func_8000E4B0(?, s8 *);                         /* extern */
? func_8000E560();                                  /* extern */
void func_800095AC(s8 *arg0, void *arg1);           /* static */
void func_80009658(s32 arg0);                       /* static */
extern s32 D_8002C3B0;
extern s8 D_80037AA0;
extern ? D_80037ADC;
extern s8 D_80037AE0;
extern u8 D_80037AE1;
extern ? D_80037B08;
extern ? D_80037B20;

s32 func_80009450(? *arg0, s8 *arg1, void *arg2) {
    ? sp6C;
    s32 sp68;
    u32 sp64;
    s32 sp60;
    ? sp40;
    ? sp28;
    s32 temp_ret;
    s32 temp_v0;
    u32 temp_v1;

    if (D_8002C3B0 != 0) {
        return 0;
    }
    D_8002C3B0 = 1;
    temp_ret = func_800073B0();
    temp_v0 = temp_ret;
    temp_v1 = (u32) (u64) temp_ret;
    sp60 = temp_v0;
    sp64 = temp_v1;
    if ((temp_v0 == 0) && (temp_v1 < 0x0165A0BCU)) {
        func_80006A00(&sp28, &sp6C, 1);
        func_8000E3D0(&sp40, 0, 0, &sp28, &sp6C);
        func_80007270(&sp28, &sp6C, 1);
    }
    D_80037AE1 = 4;
    func_80009658(0);
    func_8000E4B0(1, &D_80037AA0);
    func_80007270(arg0, &sp6C, 1);
    sp68 = func_8000E4B0(0, &D_80037AA0);
    func_80007270(arg0, &sp6C, 1);
    func_800095AC(arg1, arg2);
    D_80037AE0 = 0;
    func_8000E560();
    func_80006A00(&D_80037B08, &D_80037B20, 1);
    return sp68;
}

void func_800095AC(s8 *arg0, void *arg1) {
    ? spC;
    s32 temp_t2;
    s32 var_a2;
    s8 *var_v1;
    s8 var_v0;
    void *var_a1;

    var_a1 = arg1;
    var_v0 = 0;
    var_v1 = &D_80037AA0;
    var_a2 = 0;
    if ((s32) D_80037AE1 > 0) {
        do {
            spC.unk0 = (s32) (unaligned s32) var_v1->unk0;
            spC.unk4 = (s32) (unaligned s32) var_v1->unk4;
            temp_t2 = (s32) (spE & 0xC0) >> 4;
            var_a1->unk3 = (s8) temp_t2;
            if (!(temp_t2 & 0xFF)) {
                var_a1->unk0 = (s16) ((sp11 << 8) | sp10);
                var_v0 = (var_v0 | (1 << var_a2)) & 0xFF;
                var_a1->unk2 = sp12;
            }
            var_a2 += 1;
            var_v1 += 8;
            var_a1 += 4;
        } while (var_a2 < (s32) D_80037AE1);
    }
    *arg0 = var_v0;
}

void func_80009658(s32 arg0) {
    s8 sp13;
    s8 sp12;
    s8 sp11;
    s8 sp10;
    s8 spF;
    s8 spE;
    s8 spD;
    s8 spC;
    s32 var_v0;
    s8 *var_v1;
    s8 *var_v1_2;

    var_v1 = &D_80037AA0;
    do {
        var_v1 += 4;
        var_v1->unk-4 = 0;
    } while ((u32) var_v1 < (u32) &D_80037ADC);
    var_v1_2 = &D_80037AA0;
    D_80037AA0.unk3C = 1;
    spC = 0xFF;
    spD = 1;
    spE = 3;
    spF = arg0 & 0xFF;
    sp10 = 0xFF;
    sp11 = 0xFF;
    sp12 = 0xFF;
    sp13 = 0xFF;
    var_v0 = 0;
    if ((s32) D_80037AE1 > 0) {
        do {
            var_v0 += 1;
            var_v1_2 += 8;
            var_v1_2->unk-8 = (unaligned s32) spC.unk0;
            var_v1_2->unk-4 = (unaligned s32) spC.unk4;
        } while (var_v0 < (s32) D_80037AE1);
    }
    *var_v1_2 = 0xFE;
}
