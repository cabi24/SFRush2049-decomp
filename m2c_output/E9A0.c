s32 func_8000D5C0(?);                               /* extern */
extern ? D_8002C380;
extern ? D_8002C3A0;
extern s32 PI_BSD_DOM1_LAT_REG;
extern s32 PI_BSD_DOM1_PGS_REG;
extern s32 PI_BSD_DOM1_PWD_REG;
extern s32 PI_BSD_DOM1_RLS_REG;
extern s32 PI_BSD_DOM2_LAT_REG;
extern s32 PI_BSD_DOM2_LWD_REG;
extern s32 PI_BSD_DOM2_PGS_REG;
extern s32 PI_BSD_DOM2_RLS_REG;
extern s32 PI_CART_ADDR_REG;
extern s32 PI_DRAM_ADDR_REG;
extern s32 PI_RD_LEN_REG;
extern s32 PI_STATUS_REG;
extern s32 PI_WR_LEN_REG;

s32 func_8000DDA0(void *arg0, s32 arg1, s32 arg2, ? arg3, s32 arg4) {
    u8 temp_v0;
    u8 temp_v0_2;
    u8 temp_v0_3;
    u8 temp_v0_4;
    u8 temp_v0_5;
    u8 temp_v0_6;
    u8 temp_v0_7;
    u8 temp_v0_8;
    u8 temp_v0_9;
    void *temp_a0;

    if (PI_STATUS_REG & 3) {
        do {

        } while (PI_STATUS_REG & 3);
    }
    temp_v0 = arg0->unk9;
    temp_a0 = *(&D_8002C3A0 + (temp_v0 * 4));
    if (arg0->unk4 != temp_a0->unk4) {
        if (temp_v0 == 0) {
            temp_v0_2 = arg0->unk5;
            if (temp_v0_2 != temp_a0->unk5) {
                PI_BSD_DOM1_LAT_REG = (s32) temp_v0_2;
            }
            temp_v0_3 = arg0->unk6;
            if (temp_v0_3 != temp_a0->unk6) {
                PI_BSD_DOM1_PGS_REG = (s32) temp_v0_3;
            }
            temp_v0_4 = arg0->unk7;
            if (temp_v0_4 != temp_a0->unk7) {
                PI_BSD_DOM1_RLS_REG = (s32) temp_v0_4;
            }
            temp_v0_5 = arg0->unk8;
            if (temp_v0_5 != temp_a0->unk8) {
                PI_BSD_DOM1_PWD_REG = (s32) temp_v0_5;
            }
        } else {
            temp_v0_6 = arg0->unk5;
            if (temp_v0_6 != temp_a0->unk5) {
                PI_BSD_DOM2_LAT_REG = (s32) temp_v0_6;
            }
            temp_v0_7 = arg0->unk6;
            if (temp_v0_7 != temp_a0->unk6) {
                PI_BSD_DOM2_PGS_REG = (s32) temp_v0_7;
            }
            temp_v0_8 = arg0->unk7;
            if (temp_v0_8 != temp_a0->unk7) {
                PI_BSD_DOM2_RLS_REG = (s32) temp_v0_8;
            }
            temp_v0_9 = arg0->unk8;
            if (temp_v0_9 != temp_a0->unk8) {
                PI_BSD_DOM2_LWD_REG = (s32) temp_v0_9;
            }
        }
        temp_a0->unk4 = (u8) arg0->unk4;
        temp_a0->unk5 = (u8) arg0->unk5;
        temp_a0->unk6 = (u8) arg0->unk6;
        temp_a0->unk7 = (u8) arg0->unk7;
        temp_a0->unk8 = (u8) arg0->unk8;
    }
    PI_DRAM_ADDR_REG = func_8000D5C0(arg3);
    PI_CART_ADDR_REG = (arg0->unkC | arg2) & 0x1FFFFFFF;
    switch (arg1) {                                 /* irregular */
    case 0:
        PI_WR_LEN_REG = arg4 - 1;
block_27:
        return 0;
    case 1:
        PI_RD_LEN_REG = arg4 - 1;
        goto block_27;
    default:
        return -1;
    }
}

/*
Decompilation failure in function func_8000DF80:

Found jr instruction at E9A0.s line 329, but the corresponding jump table is not provided.

Please include it in the input .s file(s), or in an additional file.

*/

s32 func_8000E390(void) {
    if (D_8002C380.unk0 == 0) {
        return 0;
    }
    return D_8002C380.unk8;
}
