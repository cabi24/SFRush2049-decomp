s32 func_8000C4B0();                                /* extern */
? func_8000C520(s32);                               /* extern */
? func_8000CFC4(void **);                           /* extern */
? func_8000D0C4(void **, void *);                   /* extern */
void *func_8000D10C(void **);                       /* extern */
? func_8000D11C();                                  /* extern */
extern u32 D_8002C370;
extern void *D_8002C3D8;
extern void *D_8002C3E0;
extern ? D_8002D6D0;
extern s32 MI_INTR_MASK_REG;

void func_80007080(void *arg0) {
    s32 temp_s0;
    u16 temp_s1;
    void **temp_t0;

    temp_s0 = func_8000C4B0();
    temp_s1 = arg0->unk10;
    if (temp_s1 != 1) {
        if (temp_s1 == 8) {
            arg0->unk10 = 2U;
            func_8000D0C4(&D_8002C3D8, arg0);
        }
    } else {
        temp_t0 = arg0->unk8;
        if ((temp_t0 == NULL) || (temp_t0 == &D_8002C3D8)) {
            arg0->unk10 = 2U;
            func_8000D0C4(&D_8002C3D8, arg0);
        } else {
            arg0->unk10 = 8U;
            func_8000D0C4(arg0->unk8, arg0);
            func_8000D0C4(&D_8002C3D8, func_8000D10C(arg0->unk8));
        }
    }
    if (D_8002C3E0 == NULL) {
        func_8000D11C();
    } else if (D_8002C3E0->unk4 < D_8002C3D8->unk4) {
        D_8002C3E0->unk10 = 2;
        func_8000CFC4(&D_8002C3D8);
    }
    func_8000C520(temp_s0);
}

s32 func_800071D0(s32 arg0) {
    s32 var_t2;

    var_t2 = MI_INTR_MASK_REG;
    if (var_t2 != 0) {
        var_t2 |= (((u32) D_8002C370 >> 0x10) ^ -1) & 0x3F;
    }
    MI_INTR_MASK_REG = (s32) *(&D_8002D6D0 + ((u32) (arg0 & 0x3F0000 & D_8002C370) >> 0xF));
    M2C_ERROR(/* mtc0 $t4, $12 */);
    return (M2C_ERROR(/* mfc0 $12 */) & 0xFF01) | ((D_8002C370 ^ -1) & 0xFF00) | (var_t2 << 0x10);
}

s32 func_80007270(void **arg0, s32 *arg1, s32 arg2) {
    s32 temp_s0;

    temp_s0 = func_8000C4B0();
    if (arg0->unk8 == 0) {
loop_1:
        if (arg2 == 0) {
            func_8000C520(temp_s0);
            return -1;
        }
        D_8002C3E0->unk10 = 8;
        func_8000CFC4(arg0);
        if (arg0->unk8 != 0) {
            goto block_4;
        }
        goto loop_1;
    }
block_4:
    if (arg1 != NULL) {
        *arg1 = *(arg0->unk14 + (arg0->unkC * 4));
    }
    arg0->unkC = (s32) ((s32) (arg0->unkC + 1) % (s32) arg0->unk10);
    arg0->unk8 = (s32) (arg0->unk8 - 1);
    if (*arg0->unk4 != 0) {
        func_80007080(func_8000D10C(arg0 + 4));
    }
    func_8000C520(temp_s0);
    return 0;
}
