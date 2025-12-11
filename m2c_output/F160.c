? func_80006A00(? *, ? *, ?);                       /* extern */
? func_80007270(? *, ? *, ?);                       /* extern */
? func_800075E0(? *, ?, ?);                         /* extern */
s32 func_8000E4B0(?, s8 *);                         /* extern */
void func_8000E6F0(s32 arg0, s8 arg1);              /* static */
void func_8000E7B4(s32 arg0, ? *arg1);              /* static */
extern s32 D_8002C4B0;
extern s8 D_8002C4D4;
extern s8 D_80037AE0;
extern ? D_80037C90;
extern ? D_80037C98;
extern s8 D_80037F60;

void func_8000E560(void) {
    D_8002C4B0 = 1;
    func_80006A00(&D_80037C98, &D_80037C90, 1);
    func_800075E0(&D_80037C98, 0, 0);
}

void func_8000E5B0(void) {
    ? sp1C;

    if (D_8002C4B0 == 0) {
        func_8000E560();
    }
    func_80007270(&D_80037C98, &sp1C, 1);
}

void func_8000E5F4(void) {
    func_800075E0(&D_80037C98, 0, 0);
}

s32 func_8000E620(? *arg0, s32 arg1) {
    s32 sp2C;
    ? sp28;
    ? sp24;
    s32 temp_v1;
    s32 var_v0;

    D_8002C4D4 = 0xFA;
    func_8000E6F0(arg1, 0);
    func_8000E4B0(1, &D_80037F60);
    func_80007270(arg0, &sp28, 1);
    sp2C = func_8000E4B0(0, &D_80037F60);
    func_80007270(arg0, &sp28, 1);
    func_8000E7B4(arg1, &sp24);
    temp_v1 = sp26 & 1;
    if ((temp_v1 != 0) && (sp26 & 2)) {
        return 2;
    }
    if ((sp27 != 0) || (temp_v1 == 0)) {
        return 1;
    }
    var_v0 = sp2C;
    if (sp26 & 4) {
        var_v0 = 4;
    }
    return var_v0;
}

void func_8000E6F0(s32 arg0, s8 arg1) {
    s8 sp11;
    s8 sp10;
    s8 spF;
    s8 spE;
    s8 spD;
    s8 spC;
    s32 temp_a2;
    s32 var_v1;
    s8 *var_v0;

    var_v0 = &D_80037F60;
    D_80037AE0 = 0xFE;
    D_80037F60.unk3C = 1;
    spC = 1;
    spD = 3;
    spE = arg1;
    spF = 0xFF;
    sp10 = 0xFF;
    sp11 = 0xFF;
    var_v1 = 0;
    if (arg0 > 0) {
        temp_a2 = arg0 & 3;
        if (temp_a2 != 0) {
            do {
                var_v1 += 1;
                *var_v0 = 0;
                var_v0 += 1;
            } while (temp_a2 != var_v1);
            if (var_v1 != arg0) {
                goto loop_6;
            }
        } else {
            do {
loop_6:
                var_v1 += 4;
                var_v0->unk1 = 0;
                var_v0->unk2 = 0;
                var_v0->unk3 = 0;
                var_v0 += 4;
                var_v0->unk-4 = 0;
            } while (var_v1 != arg0);
        }
    }
    var_v0[6].unk-6 = (unaligned s32) spC.unk0;
    var_v0[6].unk-2 = (u8) spC.unk4;
    var_v0->unk6 = 0xFE;
    var_v0[6].unk-1 = (u8) spC.unk5;
}

void func_8000E7B4(s32 arg0, ? *arg1) {
    ? spC;
    s32 temp_a3;
    s32 temp_t1;
    s32 var_v1;
    s8 *var_v0;

    var_v0 = &D_80037F60;
    var_v1 = 0;
    if (arg0 > 0) {
        temp_a3 = arg0 & 3;
        if (temp_a3 != 0) {
            do {
                var_v1 += 1;
                var_v0 += 1;
            } while (temp_a3 != var_v1);
            if (var_v1 != arg0) {
                goto loop_6;
            }
        } else {
            do {
loop_6:
                var_v1 += 4;
                var_v0 += 4;
            } while (var_v1 != arg0);
        }
    }
    spC.unk0 = (s32) (unaligned s32) var_v0->unk0;
    spC.unk4 = (u8) var_v0->unk4;
    spC.unk5 = (u8) var_v0->unk5;
    temp_t1 = (s32) (spD & 0xC0) >> 4;
    arg1->unk3 = (s8) temp_t1;
    if (!(temp_t1 & 0xFF)) {
        arg1->unk0 = (s16) ((sp10 << 8) | spF);
        arg1->unk2 = sp11;
    }
}
