? func_80006A00(? *, ? *, ?);                       /* extern */
? func_80006E10(?, ? *, ? *, ? *);                  /* extern */
? func_80006F30(? *, ?, void (*)(void *), ? *, ? *, s32); /* extern */
? func_80007080(? *);                               /* extern */
? func_80007270(s32, u16 **, ?);                    /* extern */
? func_800075E0(s32, s32, ?);                       /* extern */
? func_80008380(?, s32);                            /* extern */
? func_8000C090();                                  /* extern */
? func_8000C11C();                                  /* extern */
s32 func_8000C490(?);                               /* extern */
s32 func_8000C4B0();                                /* extern */
? func_8000C520(s32);                               /* extern */
? func_8000C540();                                  /* extern */
void *func_8000C660();                              /* extern */
? func_8000C670();                                  /* extern */
s32 func_8000C970();                                /* extern */
void func_80006BB0(void *arg0);                     /* static */
extern ? D_8002C330;
extern s32 D_8002C34C;
extern ? D_800354F0;
extern ? D_800366A0;
extern ? D_800366B8;
extern ? D_800366D0;
extern ? D_800366E8;
extern u16 D_80036700;
extern ? D_80037C50;
extern s32 D_80037C58;
extern s32 D_80037C5C;

void func_80006A30(s32 arg0) {
    s32 sp2C;
    s32 sp28;
    s32 temp_v0;

    if (D_8002C330.unk0 == 0) {
        func_8000C090();
        D_8002C34C = 0;
        func_80006A00(&D_800366A0, &D_800366B8, 5);
        D_800366D0.unk0 = 0xD;
        D_800366D0.unk2 = 0;
        D_800366D0.unk4 = 0;
        D_800366E8.unk0 = 0xE;
        D_800366E8.unk2 = 0;
        D_800366E8.unk4 = 0;
        func_80006E10(7, &D_800366A0, &D_800366D0, &D_800366E8);
        func_80006E10(3, &D_800366A0, &D_800366E8, &D_800366E8);
        sp28 = -1;
        temp_v0 = func_8000C490(0);
        if (temp_v0 < arg0) {
            sp28 = temp_v0;
            func_80008380(0, arg0);
        }
        sp2C = func_8000C4B0();
        D_8002C330.unk0 = 1;
        D_8002C330.unk4 = &D_800354F0;
        D_8002C330.unk8 = &D_800366A0;
        D_8002C330.unkC = &D_800366A0;
        D_8002C330.unk10 = 0;
        D_8002C330.unk14 = 0;
        D_8002C330.unk18 = 0;
        func_80006F30(&D_800354F0, 0, func_80006BB0, &D_8002C330, &D_800366A0, arg0);
        func_8000C540();
        func_80007080(&D_800354F0);
        func_8000C520(sp2C);
        if (sp28 != -1) {
            func_80008380(0, sp28);
        }
    }
}

void func_80006BB0(void *arg0) {
    u16 *sp44;
    s32 temp_s0;
    s32 temp_v0_2;
    u16 temp_t7;
    u16 temp_v1;
    u16 temp_v1_2;
    u32 temp_t7_2;
    void *temp_v0;

    sp44 = NULL;
    temp_v1 = func_8000C660()->unk2;
    D_80036700 = temp_v1;
    if (temp_v1 == 0) {
        D_80036700 = 1;
    }
loop_3:
    func_80007270(arg0->unkC, &sp44, 1);
    temp_v1_2 = *sp44;
    if (temp_v1_2 == 0xD) {
        func_8000C670();
        temp_t7 = (D_80036700 - 1) & 0xFFFF;
        D_80036700 = temp_t7;
        if (temp_t7 == 0) {
            temp_v0 = func_8000C660();
            if (temp_v0->unk10 != 0) {
                func_800075E0(temp_v0->unk10, temp_v0->unk14, 0);
            }
            D_80036700 = temp_v0->unk2;
        }
        temp_s0 = D_80037C58;
        D_80037C5C += 1;
        temp_v0_2 = func_8000C970();
        temp_t7_2 = (temp_v0_2 - temp_s0) + D_80037C50.unk4;
        D_80037C58 = temp_v0_2;
        D_80037C50.unk0 = (s32) ((temp_t7_2 < (u32) D_80037C50.unk4) + D_80037C50.unk0);
        D_80037C50.unk4 = temp_t7_2;
        goto loop_3;
    }
    if (temp_v1_2 != 0xE) {
        goto loop_3;
    }
    func_8000C11C();
    goto loop_3;
}
