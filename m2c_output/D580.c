Warning: missing "jr $ra" in last block of func_8000D11C (.L8000D24C).

? func_8000FBA0(?, s64, s64, s64);                  /* extern */
void func_8000D0C4(? *arg0, ? *arg1);               /* static */
void func_8000D11C();                               /* static */
extern u32 D_8002C370;
extern ? *D_8002C3D8;
extern ? *D_8002C3E0;
extern ? D_8002D6D0;
extern s32 MI_INTR_MASK_REG;
extern s32 SP_STATUS_REG;

/*
Decompilation failure in function func_8000C980:

Unable to determine jump table for jr instruction at D580.s line 17.

There must be a read of a variable before the instruction
which has a name starting with with "jtbl"/"jpt_"/"lbl_"/"jumptable_".
*/

/*
Decompilation failure in function func_8000CEDC:

Unable to parse branch: label .L8000CE94 does not exist in function func_8000CEDC
*/

void func_8000CFC4(s32 arg0) {
    s32 temp_k1;
    s32 temp_t1;
    s32 var_k1;

    D_8002C3E0->unk118 = (s32) (M2C_ERROR(/* mfc0 $12 */) | 2);
    D_8002C3E0->unk98 = (s64) saved_reg_s0;
    D_8002C3E0->unkA0 = (s64) saved_reg_s1;
    D_8002C3E0->unkA8 = (s64) saved_reg_s2;
    D_8002C3E0->unkB0 = (s64) saved_reg_s3;
    D_8002C3E0->unkB8 = (s64) saved_reg_s4;
    D_8002C3E0->unkC0 = (s64) saved_reg_s5;
    D_8002C3E0->unkC8 = (s64) saved_reg_s6;
    D_8002C3E0->unkD0 = (s64) saved_reg_s7;
    D_8002C3E0->unkE8 = (s64) saved_reg_gp;
    D_8002C3E0->unkF0 = (s64) sp;
    D_8002C3E0->unkF8 = (s64) saved_reg_fp;
    D_8002C3E0->unk100 = (s64) saved_reg_ra;
    D_8002C3E0->unk11C = (s32) saved_reg_ra;
    if (D_8002C3E0->unk18 != 0) {
        D_8002C3E0->unk180 = (f64) saved_reg_f20;
        D_8002C3E0->unk188 = (f64) saved_reg_f22;
        D_8002C3E0->unk190 = (f64) saved_reg_f24;
        D_8002C3E0->unk198 = (f64) saved_reg_f26;
        D_8002C3E0->unk1A0 = (f64) saved_reg_f28;
        D_8002C3E0->unk1A8 = (f64) saved_reg_f30;
        D_8002C3E0->unk12C = (s32) M2C_ERROR(/* cfc1 */);
    }
    temp_k1 = D_8002C3E0->unk118;
    temp_t1 = temp_k1 & 0xFF00;
    if (temp_t1 != 0) {
        D_8002C3E0->unk118 = (s32) ((temp_k1 & 0xFFFF00FF) | (temp_t1 | ((D_8002C370 ^ -1) & 0xFF00)));
    }
    var_k1 = MI_INTR_MASK_REG;
    if (var_k1 != 0) {
        var_k1 |= (((u32) D_8002C370 >> 0x10) ^ -1) & 0x3F & D_8002C3E0->unk128;
    }
    D_8002C3E0->unk128 = var_k1;
    if (arg0 != 0) {
        func_8000D0C4(D_8002C3E0);
    }
    func_8000D11C();
}

void func_8000D0C4(? *arg0, ? *arg1) {
    ? *var_t8;
    ? *var_t9;
    s32 temp_t7;

    var_t8 = *arg0;
    temp_t7 = arg1->unk4;
    var_t9 = arg0;
    if (var_t8->unk4 >= temp_t7) {
        do {
            var_t9 = var_t8;
            var_t8 = *var_t8;
        } while (var_t8->unk4 >= temp_t7);
    }
    arg1->unk0 = *var_t9;
    *var_t9 = arg1;
    arg1->unk8 = arg0;
}

void func_8000D10C(? **arg0) {
    *arg0 = **arg0;
}

void func_8000D11C(void) {
    ? *temp_v0;

    temp_v0 = func_8000D10C(&D_8002C3D8);
    D_8002C3E0 = temp_v0;
    temp_v0->unk10 = 4;
    M2C_ERROR(/* mtc0 $k1, $12 */);
    M2C_ERROR(/* mtc0 $k1, $14 */);
    if (temp_v0->unk18 != 0) {

    }
    MI_INTR_MASK_REG = (s32) *(((temp_v0->unk128 & ((u32) D_8002C370 >> 0x10)) * 2) + &D_8002D6D0);
    M2C_ERROR(/* unknown instruction: eret */);
    func_8000FBA0(0, temp_v0->unk40, temp_v0->unk48, temp_v0->unk50);
}

s32 func_8000D2A0(void) {
    return SP_STATUS_REG;
}
