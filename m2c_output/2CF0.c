void D_800AC75C(s32 arg0);                          /* extern */
void D_800E7808(s32 arg0);                          /* extern */
? func_80000450(? *, void *, ?, s32, s32);          /* extern */
? func_800005D4(? *, ? *, ? *);                     /* extern */
s32 func_80001DFC();                                /* extern */
? func_800049F0(?);                                 /* extern */
? func_80006814(? *, ? *, ?);                       /* extern */
? func_80006A00(? *, ? *, ?);                       /* extern */
? func_80006F30(? *, ?, void (*)(s32), s32, void *, s32); /* extern */
? func_80007080(? *);                               /* extern */
? func_80007270(? *, s16 **, ?);                    /* extern */
? func_800075E0(? *, ?, ?);                         /* extern */
? func_80007E80();                                  /* extern */
? func_800081D0(s32, s32 *);                        /* extern */
? func_80008210(?, ? *, ? *, ?);                    /* extern */
? func_80008380(?, ?);                              /* extern */
? func_80008460(? *, s32);                          /* extern */
? func_800084E0(? *, s32);                          /* extern */
? func_80008590(? *, s32);                          /* extern */
? func_800A48C8();                                  /* extern */
? func_800A4934();                                  /* extern */
? func_800EE5DC();                                  /* extern */
? func_800EEA7C();                                  /* extern */
? func_800FD464();                                  /* extern */
void func_800021A4(s32 arg0);                       /* static */
void func_80002238(s32 arg0);                       /* static */
void func_800024FC(s32 arg0);                       /* static */
extern s8 D_8002B028;
extern ? D_8002E8E8;
extern ? D_8002ECC0;
extern ? D_8002ECD8;
extern ? D_8002ECF8;
extern ? D_8002ED10;
extern ? D_8002EE08;
extern ? D_8002EE20;
extern ? D_8002F140;
extern ? D_8002F160;
extern ? D_8002F180;
extern ? D_8002F660;
extern ? D_8002F7F0;
extern ? D_80030150;
extern ? D_80030AB0;
extern ? D_80031D70;
extern ? D_80033EA0;
extern ? D_800344E0;
extern ? D_80034690;
extern ? D_80034840;
extern ? D_80034BA0;
extern ? D_80034D50;
extern ? D_80035410;
extern ? D_80035428;
extern ? D_80035440;
extern s8 D_80035472;
extern ? D_80086A50;
extern ? D_8010FD80;
extern ? D_801249F0;
extern s16 D_80142D90;
extern ? D_8017A640;
extern ? D_B0CB10;

void func_800020F0(s32 arg0) {
    u32 sp64;
    s32 sp60;
    u32 temp_t1;

    func_80007E80();
    sp60 = 0xFFB000;
    sp64 = 0;
    do {
        func_800081D0(sp60, &(&sp20[0])[sp64]);
        temp_t1 = sp64 + 1;
        sp60 += 4;
        sp64 = temp_t1;
    } while (temp_t1 < 0x10U);
    func_80006F30(&D_80034BA0, 1, func_800021A4, arg0, &D_8002F660 + 0x190, 2);
    func_80007080(&D_80034BA0);
}

void func_800021A4(s32 arg0) {
    func_80008210(0x96, &D_8002EE08, &D_8002EE20, 0xC8);
    func_80008380(0, 0);
    func_80006F30(&D_80034D50, 6, func_80002238, arg0, &D_80030150 + 0x960, 4);
    func_80007080(&D_80034D50);
loop_1:
    goto loop_1;
}

void func_80002238(s32 arg0) {
    ? sp30;
    s16 *sp2C;

    sp2C = NULL;
    func_800049F0(0x01000E00);
    func_80006A00(&D_80035410, &D_8002F140, 8);
    func_80006A00(&D_80035428, &D_8002F160, 8);
    func_80006A00(&D_8002ECF8, &D_8002ED10, 0x3C);
    func_80006A00(&D_8002ECC0, &D_8002ECD8, 8);
    func_80000450(&D_8002E8E8, &D_80033EA0 + 0x640, 0xC, func_80001DFC(), 1);
    func_80008460(&D_80086A50, &D_8010FD80 - &D_80086A50);
    func_800084E0(&D_8010FD80, &D_801249F0 - &D_8010FD80);
    func_80006814(&D_B0CB10, &D_80086A50, 0);
    func_80008590(&D_801249F0, &D_8017A640 - &D_801249F0);
    func_800005D4(&D_8002E8E8, &sp30, &D_8002ECF8);
    func_80006F30(&D_80034690, 8, D_800AC75C, 0, &D_8002F7F0 + 0x960, 3);
    func_80006A00(&D_80035440, &D_8002F180, 1);
    func_800075E0(&D_80035440, 0, 1);
    func_80007080(&D_80034690);
    func_80006F30(&D_80034840, 5, D_800E7808, 0, &D_80031D70 + 0x960, 7);
    func_80007080(&D_80034840);
    func_800EEA7C();
    func_800A4934();
    func_80006F30(&D_800344E0, 7, func_800024FC, arg0, &D_80030AB0 + 0x12C0, 5);
    D_80142D90 = 0xABF;
    if (D_8002B028 == 0) {
        func_80007080(&D_800344E0);
    }
loop_2:
    func_80007270(&D_8002ECF8, &sp2C, 1);
    if (*sp2C != 0xABE) {
        goto loop_2;
    }
    D_80035472 = 0;
    func_800A48C8();
loop_6:
    func_800EE5DC();
    goto loop_6;
}

void func_800024FC(s32 arg0) {
    func_800049F0(0x01000E00);
loop_1:
    func_800FD464();
    goto loop_1;
}
