? func_80001D60();                                  /* extern */
? func_80006A00(void *, void *, ?);                 /* extern */
? func_80006A30(?);                                 /* extern */
? func_80006D40(void *);                            /* extern */
? func_80006DA0(?);                                 /* extern */
? func_80006E10(?, void *, ?);                      /* extern */
? func_80006ED0(void *, ?, u8);                     /* extern */
? func_80006F30(void *, ?, ? (*)(), void *, s32, s32); /* extern */
? func_80007080(void *);                            /* extern */
s32 func_800071D0(s32);                             /* extern */
s32 func_80007270(void *, ? **, ?);                 /* extern */
u32 func_800073B0();                                /* extern */
? func_80007590(s32);                               /* extern */
s32 func_800075E0(s32, void *, ?);                  /* extern */
s32 func_80007730(void *);                          /* extern */
s32 func_80007790();                                /* extern */
s32 func_800077D0();                                /* extern */
? func_80007810();                                  /* extern */
? func_8000794C(void *);                            /* extern */
? func_80007AB4(void *);                            /* extern */
s32 func_80007B00(s32);                             /* extern */
? func_80007B80();                                  /* extern */
? func_8000063C();                                  /* static */
s32 func_80000C54(void *arg0, void *arg1);          /* static */
void func_80000D6C(void *arg0, ? *arg1);            /* static */
void func_80000E20(void *arg0, void *arg1, void *arg2); /* static */
void func_80000F88(void *arg0);                     /* static */
s32 func_80000FE8(void *, void **, void **, s32);   /* static */
extern void *D_8002AFA0;
extern s32 D_8002AFA4;
extern s32 D_8002AFA8;
extern s32 D_8002AFB0;
extern f32 D_8002AFB4;
extern f32 D_8002AFB8;
extern ? D_8002B1B0;
extern s32 D_8002E8E0;
extern s32 D_8002EB64;
extern s16 D_8002EB70;
extern u32 D_8002EB78;
extern s32 D_8002EB80;
extern s32 D_8002EB84;
extern f32 D_8002EB90;
extern f32 D_8002EB94;
extern s32 D_8002EB98;
extern s32 D_8002EB9C;
extern s32 D_8002EBA0;
extern s32 D_8002EBA4;

void func_80000450(void *arg0, s32 arg1, s32 arg2, u8 arg3, u8 arg4) {
    D_8002EB70 = 0;
    arg0->unk274 = 0;
    arg0->unk278 = 0;
    arg0->unk260 = 0;
    arg0->unk27C = 0;
    arg0->unk264 = 0;
    arg0->unk268 = 0;
    arg0->unk26C = 0;
    arg0->unk270 = 0;
    arg0->unk0 = 1;
    arg0->unk20 = 4;
    func_80006A00(arg0 + 0x40, arg0 + 0x58, 8);
    func_80006A00(arg0 + 0x78, arg0 + 0x90, 8);
    func_80006A30(0xFE);
    func_80006D40((arg3 * 0x50) + &D_8002B1B0);
    func_80006DA0(1);
    func_80006E10(4, arg0 + 0x40, 0x29B);
    func_80006E10(9, arg0 + 0x40, 0x29C);
    func_80006E10(0xE, arg0 + 0x40, 0x29D);
    func_80006E10(0, arg0 + 0x40, 0x29E);
    func_80006ED0(arg0 + 0x40, 0x29A, arg4);
    func_80006F30(arg0 + 0xB0, 4, func_8000063C, arg0, arg1, arg2);
    func_80007080(arg0 + 0xB0);
}

void func_800005D4(void *arg0, void *arg1, s32 arg2) {
    s32 sp1C;

    sp1C = func_800071D0(1);
    arg1->unk4 = arg2;
    arg1->unk0 = (void *) arg0->unk260;
    arg0->unk260 = arg1;
    func_800071D0(sp1C);
}

/*
Decompilation failure in function func_8000063C:

Found jr instruction at 1050.s line 169, but the corresponding jump table is not provided.

Please include it in the input .s file(s), or in an additional file.

*/

void func_80000780(void *arg0) {
    ? *sp24;
    s32 sp20;
    void *sp1C;
    void *sp18;

    sp1C = NULL;
    sp18 = NULL;
    if (func_80007270(arg0 + 0x78, &sp24, 0) != -1) {
        do {
            func_80000D6C(arg0, sp24);
        } while (func_80007270(arg0 + 0x78, &sp24, 0) != -1);
    }
    if ((arg0->unk280 != 0) && (arg0->unk264 != 0) && (arg0->unk274 != 0)) {
        func_80000F88(arg0);
    } else {
        sp20 = ((arg0->unk274 == 0) * 2) | (arg0->unk278 == 0);
        if (func_80000FE8(arg0, &sp1C, &sp18, sp20) != sp20) {
            func_80000E20(arg0, sp1C, sp18);
        }
    }
}

void func_80000880(void *arg0) {
    void *sp28;
    s32 sp20;
    s32 sp1C;
    void *temp_t0;
    void *temp_t8;

    sp20 = 0;
    sp1C = 0;
    arg0->unk27C = (s32) (arg0->unk27C + 1);
    if ((D_8002AFA4 != 0) && ((u32) (arg0->unk27C - D_8002AFA8) >= 2U)) {
        func_80007590(D_8002AFA4);
        func_80001D60();
        D_8002AFA8 = arg0->unk27C;
        D_8002AFA4 = 0;
    }
    temp_t8 = arg0->unk260;
    sp28 = temp_t8;
    if (temp_t8 != NULL) {
        do {
            func_800075E0(sp28->unk4, arg0, 0);
            temp_t0 = sp28->unk0;
            sp28 = temp_t0;
        } while (temp_t0 != NULL);
    }
}

void func_80000950(void *arg0) {
    void *sp24;
    void *sp20;
    void *sp1C;
    s32 sp18;

    sp20 = NULL;
    sp1C = NULL;
    if (arg0->unk274 == NULL) {

    }
    sp24 = arg0->unk274;
    arg0->unk274 = NULL;
    if ((sp24->unk4 & 0x10) && (func_80007730(sp24 + 0x10) != 0)) {
        sp24->unk4 = (s32) (sp24->unk4 | 0x20);
        if ((sp24->unk8 & 7) == 3) {
            sp24->unk0 = (void *) arg0->unk268;
            arg0->unk268 = sp24;
            if (arg0->unk270 == NULL) {
                arg0->unk270 = sp24;
            }
        }
    } else {
        if (sp24->unk8 & 0x40) {

        } else if (sp24->unk10 == 2) {

        }
        sp24->unk4 = (s32) (sp24->unk4 & ~2);
        func_80000C54(arg0, sp24);
    }
    sp18 = ((arg0->unk274 == NULL) * 2) | (arg0->unk278 == 0);
    if (func_80000FE8(arg0, &sp20, &sp1C, sp18) != sp18) {
        func_80000E20(arg0, sp20, sp1C);
    }
}

void func_80000ACC(void *arg0) {
    void *sp24;
    void *sp20;
    void *sp1C;
    s32 sp18;

    sp20 = NULL;
    sp1C = NULL;
    if (arg0->unk278 == NULL) {

    }
    if (arg0->unk278->unk10 != 1) {

    }
    sp24 = arg0->unk278;
    arg0->unk278 = NULL;
    sp24->unk4 = (s32) (sp24->unk4 & ~1);
    func_80000C54(arg0, sp24);
    sp18 = ((arg0->unk274 == 0) * 2) | (arg0->unk278 == NULL);
    if (func_80000FE8(arg0, &sp20, &sp1C, sp18) != sp18) {
        func_80000E20(arg0, sp20, sp1C);
    }
}

s32 func_80000BA4(void *arg0, s32 arg1) {
    s32 sp2C;
    s32 sp28;
    s32 temp_v0;

    if (arg1 != 0) {
        temp_v0 = func_80007790();
        sp2C = temp_v0;
        sp28 = func_800077D0();
        if (sp28 != temp_v0) {
            return 0;
        }
        if ((D_8002AFA4 != 0) && ((u32) (arg0->unk27C - D_8002AFA8) < 2U)) {
            return 0;
        }
        return arg1;
    }
    return 0;
}

s32 func_80000C54(void *arg0, void *arg1) {
    s32 sp1C;
    s32 temp_t7;

    if (!(arg1->unk4 & 3)) {
        if (arg1->unk50 == 0) {

        }
        sp1C = func_800075E0(arg1->unk50, arg1->unk54, 1);
        if (arg1->unk10 == 1) {
            D_8002EB70 -= 1;
            temp_t7 = arg1->unk8;
            if ((temp_t7 & 0x40) && (temp_t7 & 0x20)) {
                if ((u32) (arg0->unk27C - D_8002AFA8) >= 2U) {
                    D_8002AFA8 = arg0->unk27C;
                    func_80007590(arg1->unkC);
                    func_80001D60();
                } else {
                    D_8002AFA4 = arg1->unkC;
                }
            }
        }
        return 1;
    }
    return 0;
}

void func_80000D6C(void *arg0, ? *arg1) {
    s32 sp4;

    sp4 = arg1->unk10;
    if ((sp4 != 2) && (sp4 != 1)) {

    }
    if (sp4 == 2) {
        if (arg0->unk26C != NULL) {
            *arg0->unk26C = arg1;
        } else {
            arg0->unk264 = arg1;
        }
        arg0->unk26C = arg1;
        if (arg0->unk280 != 0) {

        }
        arg0->unk280 = 1;
    } else {
        if (arg0->unk270 != NULL) {
            *arg0->unk270 = arg1;
        } else {
            arg0->unk268 = arg1;
        }
        arg0->unk270 = arg1;
    }
    arg1->unk0 = NULL;
    arg1->unk4 = (s32) (arg1->unk8 & 3);
}

void func_80000E20(void *arg0, void *arg1, void *arg2) {
    s32 sp1C;
    u32 temp_ret;

    if (arg0->unk274 != NULL) {

    }
    if (arg1 != NULL) {
        if (arg1->unk10 == 2) {
            temp_ret = func_800073B0();
            D_8002EB78 = temp_ret;
            D_8002EB78 = (u32) (u64) temp_ret;
            D_8002AFA0 = arg1;
        } else if (!(arg1->unk4 & 0x20)) {
            D_8002E8E0 = D_8002EB80;
            D_8002E8E0 = D_8002EB84;
        }
        func_80007810();
        arg1->unk4 = (s32) (arg1->unk4 & ~0x30);
        func_8000794C(arg1 + 0x10);
        func_80007AB4(arg1 + 0x10);
        arg0->unk274 = arg1;
        if (arg1 == arg2) {
            arg0->unk278 = arg2;
        }
    }
    if ((arg2 != NULL) && (arg2 != arg1)) {
        if (arg2->unk38 == 0) {

        }
        sp1C = func_80007B00(arg2->unk38);
        if (sp1C != 0) {

        }
        arg0->unk278 = arg2;
    }
}

void func_80000F88(void *arg0) {
    void *temp_t0;

    if (arg0->unk274->unk10 == 1) {
        temp_t0 = arg0->unk274;
        temp_t0->unk4 = (s32) (temp_t0->unk4 | 0x10);
        func_80007B80();
    }
}

/*
Decompilation failure in function func_80000FE8:

Found jr instruction at 1050.s line 949, but the corresponding jump table is not provided.

Please include it in the input .s file(s), or in an additional file.

*/

void func_80001350(void) {
    s32 sp24;
    f32 temp_fs0;

    sp24 = func_800071D0(1);
    D_8002EBA4 = D_8002EB64;
    temp_fs0 = 2.0f * D_8002AFB8;
    D_8002EB94 = temp_fs0;
    D_8002EB90 = temp_fs0;
    func_800071D0(sp24);
}

void func_800013C0(void) {
    D_8002AFB0 = 1;
}

void func_800013DC(void) {
    D_8002AFB0 = 0;
}

void func_800013F4(void) {
    D_8002EB98 = D_8002EB64 - D_8002EBA4;
    D_8002EBA4 = D_8002EB64;
    if (D_8002AFB0 != 0) {
        D_8002EBA0 += D_8002EB98;
        D_8002EB9C += D_8002EB98;
    } else {
        D_8002EB90 = (f32) (D_8002EB64 - D_8002EBA0) * D_8002AFB8;
    }
    if (D_8002EB98 >= 6) {
        D_8002EB98 = 5;
    }
    D_8002EB94 = (f32) D_8002EB98 * D_8002AFB8;
}

void func_800014F0(f32 arg0) {
    D_8002EBA0 = D_8002EB64;
    D_8002EB9C = (s32) (arg0 * D_8002AFB4) + D_8002EB64;
}

void func_8000153C(f32 arg0) {
    D_8002EB9C += (s32) (arg0 * D_8002AFB4);
}

f32 func_80001578(void) {
    return (f32) (D_8002EB9C - D_8002EB64) * D_8002AFB8;
}

s32 func_800015BC(void) {
    return (D_8002EB9C - D_8002EB64) < 1;
}

void func_800015DC(void) {

}
