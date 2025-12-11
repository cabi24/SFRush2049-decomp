? func_80007CA0(?, ?);                              /* extern */
? func_80008460(?, ?);                              /* extern */
? func_80008590(? *, ?);                            /* extern */
? func_8000D770(s32);                               /* extern */
s32 func_8000D780();                                /* extern */
? func_8000D790(?);                                 /* extern */
s32 func_8000D7A0(?, s32 *);                        /* extern */
s32 func_8000D7F0(?, s32);                          /* extern */
? func_8000D840();                                  /* extern */
? func_8000D890();                                  /* extern */
u32 func_8000D958(s32, u32, ?, ?);                  /* extern */
s32 func_8000DA58(u32, s32, ?, ?);                  /* extern */
s32 func_8000DBB0();                                /* extern */
void func_80008128();                               /* static */
extern s32 AI_BITRATE_REG;
extern s32 AI_CONTROL_REG;
extern s32 AI_DACRATE_REG;
extern s32 D_80000000;
extern u32 D_8002C360;
extern s32 D_8002C364;
extern s32 D_8002C368;
extern s32 D_800367D0;
extern s8 D_800367E4;
extern s8 D_800367E5;
extern s8 D_800367E6;
extern s8 D_800367E7;
extern s8 D_800367E8;
extern s8 D_8003685C;
extern s8 D_8003685D;
extern s8 D_8003685E;
extern s8 D_8003685F;
extern s8 D_80036860;
extern s32 PI_BSD_DOM1_LAT_REG;
extern s32 PI_BSD_DOM1_PGS_REG;
extern s32 PI_BSD_DOM1_PWD_REG;
extern s32 PI_BSD_DOM1_RLS_REG;
extern s32 PI_BSD_DOM2_LAT_REG;
extern s32 PI_BSD_DOM2_LWD_REG;
extern s32 PI_BSD_DOM2_PGS_REG;
extern s32 PI_BSD_DOM2_RLS_REG;
extern ? func_8000C980;
extern ? osAppNMIBuffer;
extern s32 osResetType;
extern s32 osTvType;

void func_80007E80(void) {
    s32 sp34;
    s32 sp30;
    u32 sp24;
    s32 sp20;
    s32 temp_ret;
    u32 temp_ret_2;

    sp30 = 0;
    D_800367D0 = 1;
    func_8000D770(func_8000D780() | 0x20000000);
    func_8000D790(0x01000800);
    if (func_8000D7A0(0x1FC007FC, &sp34) != 0) {
        do {

        } while (func_8000D7A0(0x1FC007FC, &sp34) != 0);
    }
    if (func_8000D7F0(0x1FC007FC, sp34 | 8) != 0) {
        do {

        } while (func_8000D7F0(0x1FC007FC, sp34 | 8) != 0);
    }
    D_80000000 = func_8000C980.unk0;
    D_80000000 = func_8000C980.unk4;
    D_80000000 = func_8000C980.unk8;
    D_80000000 = func_8000C980.unkC;
    (void *)0x80000080->unk0 = (s32) func_8000C980.unk0;
    (void *)0x80000080->unk4 = (s32) func_8000C980.unk4;
    (void *)0x80000080->unk8 = (s32) func_8000C980.unk8;
    (void *)0x80000080->unkC = (s32) func_8000C980.unkC;
    (void *)0x80000100->unk0 = (s32) func_8000C980.unk0;
    (void *)0x80000100->unk4 = (s32) func_8000C980.unk4;
    (void *)0x80000100->unk8 = (s32) func_8000C980.unk8;
    (void *)0x80000100->unkC = (s32) func_8000C980.unkC;
    (void *)0x80000180->unk0 = (s32) func_8000C980.unk0;
    (void *)0x80000180->unk4 = (s32) func_8000C980.unk4;
    (void *)0x80000180->unk8 = (s32) func_8000C980.unk8;
    (void *)0x80000180->unkC = (s32) func_8000C980.unkC;
    func_80007CA0(0x80000000, 0x190);
    func_80008460(0x80000000, 0x190);
    func_80008128();
    func_8000D840();
    func_8000D890();
    temp_ret = func_8000DA58(D_8002C360, D_8002C364, 0, 3);
    sp20 = temp_ret;
    sp24 = (u32) (u64) temp_ret;
    temp_ret_2 = func_8000D958(sp20, sp24, 0, 4);
    D_8002C360 = temp_ret_2;
    D_8002C360 = (u32) (u64) temp_ret_2;
    if (osResetType == 0) {
        func_80008590(&osAppNMIBuffer, 0x40);
    }
    if (osTvType == 0) {
        D_8002C368 = 0x02F5B2D2;
    } else if (osTvType == 2) {
        D_8002C368 = 0x02E6025C;
    } else {
        D_8002C368 = 0x02E6D354;
    }
    if (func_8000DBB0() & 0x1000) {
loop_12:
        goto loop_12;
    }
    AI_CONTROL_REG = 1;
    AI_DACRATE_REG = 0x3FFF;
    AI_BITRATE_REG = 0xF;
}

void func_80008128(void) {
    D_800367E4 = 7;
    D_800367E5 = (s8) PI_BSD_DOM1_LAT_REG;
    D_800367E8 = (s8) PI_BSD_DOM1_PWD_REG;
    D_800367E6 = (s8) PI_BSD_DOM1_PGS_REG;
    D_800367E7 = (s8) PI_BSD_DOM1_RLS_REG;
    D_8003685C = 7;
    D_8003685D = (s8) PI_BSD_DOM2_LAT_REG;
    D_80036860 = (s8) PI_BSD_DOM2_LWD_REG;
    D_8003685E = (s8) PI_BSD_DOM2_PGS_REG;
    D_8003685F = (s8) PI_BSD_DOM2_RLS_REG;
}
