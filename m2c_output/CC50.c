? func_800075E0(s32, s32, ?);                       /* extern */
s32 func_8000C4B0();                                /* extern */
? func_8000C520(s32);                               /* extern */
s32 func_8000C970();                                /* extern */
? func_8000FB90(u32);                               /* extern */
void func_8000C294(u32 arg0, u32 arg1);             /* static */
u32 func_8000C308(? *arg0);                         /* static */
extern void *D_8002C3E0;
extern ? *D_8002C3F0;
extern s32 D_80037C54;
extern s32 D_80037C58;
extern s32 D_80037C5C;
extern s32 D_80037C60;

void func_8000C050(? *arg0, ? **arg1) {
    ? *var_a2;
    ? *var_a3;

    var_a2 = arg0;
    var_a3 = *var_a2;
    if (var_a3 != NULL) {
loop_1:
        if (var_a3 == arg1) {
            *var_a2 = *arg1;
            return;
        }
        var_a2 = var_a3;
        var_a3 = *var_a2;
        if (var_a3 == NULL) {

        } else {
            goto loop_1;
        }
    }
}

void func_8000C090(void) {
    D_80037C54 = 0;
    D_80037C54 = 0;
    D_80037C58 = 0;
    D_80037C5C = 0;
    D_8002C3F0->unk4 = (? *) D_8002C3F0;
    D_8002C3F0->unk0 = D_8002C3F0->unk4;
    D_8002C3F0->unk10 = 0;
    D_8002C3F0->unk14 = 0;
    D_8002C3F0->unk8 = (s32) D_8002C3F0->unk10;
    D_8002C3F0->unkC = (s32) D_8002C3F0->unk14;
    D_8002C3F0->unk18 = 0;
    D_8002C3F0->unk1C = 0;
}

void func_8000C11C(void) {
    ? *sp24;
    s32 sp20;
    u32 sp1C;
    ? *sp18;
    ? *temp_t9;
    s32 temp_t6_2;
    u32 temp_t3;
    u32 temp_t6;
    u32 temp_t8;
    u32 temp_t9_2;

    if (*D_8002C3F0 != D_8002C3F0) {
loop_1:
        temp_t9 = *D_8002C3F0;
        sp24 = temp_t9;
        if (temp_t9 == D_8002C3F0) {
            func_8000FB90(0U);
            D_80037C60 = 0;
            return;
        }
        sp20 = func_8000C970();
        sp1C = sp20 - D_80037C60;
        D_80037C60 = sp20;
        temp_t6 = sp24->unk10;
        if ((temp_t6 >= 0U) && ((temp_t6 > 0U) || (sp1C < (u32) sp24->unk14))) {
            sp18 = sp24 + 0x10;
            temp_t3 = sp24->unk14;
            sp24->unk14 = (u32) (temp_t3 - sp1C);
            sp24->unk10 = (u32) ((sp24->unk10 - 0) - (temp_t3 < sp1C));
            func_8000C294(sp24->unk10, sp24->unk14);
            return;
        }
        *sp24->unk4 = sp24->unk0;
        sp24->unk0->unk4 = (? **) sp24->unk4;
        sp24->unk0 = NULL;
        sp24->unk4 = NULL;
        temp_t6_2 = sp24->unk18;
        if (temp_t6_2 != 0) {
            func_800075E0(temp_t6_2, sp24->unk1C, 0);
        }
        temp_t8 = sp24->unk8;
        temp_t9_2 = sp24->unkC;
        if ((temp_t8 != 0) || (temp_t9_2 != 0)) {
            sp24->unk10 = temp_t8;
            sp24->unk14 = temp_t9_2;
            func_8000C308(sp24);
        }
        goto loop_1;
    }
}

void func_8000C294(u32 arg0, u32 arg1) {
    u32 sp24;
    s32 sp20;
    s32 sp1C;
    u32 temp_t3;

    sp1C = func_8000C4B0();
    D_80037C60 = func_8000C970();
    temp_t3 = D_80037C60 + arg1;
    sp20 = (temp_t3 < arg1) + arg0;
    sp24 = temp_t3;
    func_8000FB90(temp_t3);
    func_8000C520(sp1C);
}

u32 func_8000C308(? *arg0) {
    ? *sp34;
    u32 sp2C;
    u32 sp28;
    s32 sp24;
    u32 *sp20;
    ? *sp1C;
    ? *temp_t3;
    ? *temp_t7;
    u32 temp_t0;
    u32 temp_t1;
    u32 temp_t2;
    u32 temp_t5;
    u32 temp_t5_2;
    u32 temp_t6;
    u32 temp_t7_2;

    sp24 = func_8000C4B0();
    temp_t7 = *D_8002C3F0;
    sp34 = temp_t7;
    temp_t1 = arg0->unk14;
    temp_t0 = arg0->unk10;
    sp2C = temp_t1;
    sp28 = temp_t0;
    if (temp_t7 != D_8002C3F0) {
        temp_t2 = temp_t7->unk10;
        if ((temp_t0 >= temp_t2) && ((temp_t2 < temp_t0) || ((u32) temp_t7->unk14 < temp_t1))) {
loop_4:
            sp20 = &sp28;
            temp_t5 = sp28.unk4;
            temp_t7_2 = sp34->unk14;
            sp28.unk4 = (u32) (temp_t5 - temp_t7_2);
            sp28.unk0 = (sp28.unk0 - sp34->unk10) - (temp_t5 < temp_t7_2);
            temp_t3 = sp34->unk0;
            sp34 = temp_t3;
            if (temp_t3 != D_8002C3F0) {
                temp_t6 = temp_t3->unk10;
                if ((temp_t6 < sp28) || ((sp28 >= temp_t6) && ((u32) temp_t3->unk14 < sp2C))) {
                    goto loop_4;
                }
            }
        }
    }
    arg0->unk10 = sp28;
    arg0->unk14 = sp2C;
    if (sp34 != D_8002C3F0) {
        sp1C = sp34 + 0x10;
        temp_t5_2 = sp34->unk14;
        sp34->unk14 = (u32) (temp_t5_2 - sp2C);
        sp34->unk10 = (s32) ((sp34->unk10 - sp28) - (temp_t5_2 < sp2C));
    }
    arg0->unk0 = sp34;
    arg0->unk4 = (? *) sp34->unk4;
    *sp34->unk4 = arg0;
    sp34->unk4 = arg0;
    func_8000C520(sp24);
    return sp28;
}

s32 func_8000C490(void *arg0) {
    void *var_a0;

    var_a0 = arg0;
    if (var_a0 == NULL) {
        var_a0 = D_8002C3E0;
    }
    return var_a0->unk4;
}
