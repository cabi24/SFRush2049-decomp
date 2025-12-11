? func_80006A00(? *, ? *, ?);                       /* extern */
? func_80006E10(?, ? *, ?);                         /* extern */
? func_80006F30(? *, ?, ? *, ? *, ? *, s32);        /* extern */
? func_80007080(? *);                               /* extern */
? func_8000C050(s32, void *);                       /* extern */
s32 func_8000C490(?);                               /* extern */
s32 func_8000C4B0();                                /* extern */
? func_8000C520(s32);                               /* extern */
? func_8000CFC4(void **);                           /* extern */
? func_8000D0C4(s32, void *);                       /* extern */
? func_8000DBC0();                                  /* extern */
void func_80008380(void *arg0, s32 arg1);           /* static */
extern ? D_8002C380;
extern void *D_8002C3D8;
extern void *D_8002C3E0;
extern s32 D_8002C4A0;
extern ? D_800368D0;
extern ? D_80037A80;
extern ? D_80037A98;
extern ? D_80037C78;
extern ? func_8000DCD0;
extern ? func_8000DDA0;
extern ? func_8000DF80;

void func_80008210(s32 arg0, ? *arg1, ? *arg2, ? arg3) {
    s32 sp2C;
    s32 sp28;
    s32 temp_v0;

    if (D_8002C380.unk0 == 0) {
        func_80006A00(arg1, arg2, arg3);
        func_80006A00(&D_80037A80, &D_80037A98, 1);
        if (D_8002C4A0 == 0) {
            func_8000DBC0();
        }
        func_80006E10(8, &D_80037A80, 0x22222222);
        sp28 = -1;
        temp_v0 = func_8000C490(0);
        if (temp_v0 < arg0) {
            sp28 = temp_v0;
            func_80008380(NULL, arg0);
        }
        sp2C = func_8000C4B0();
        D_8002C380.unk0 = 1;
        D_8002C380.unk4 = &D_800368D0;
        D_8002C380.unkC = &D_80037A80;
        D_8002C380.unk10 = &D_80037C78;
        D_8002C380.unk14 = &func_8000DCD0;
        D_8002C380.unk18 = &func_8000DDA0;
        D_8002C380.unk8 = arg1;
        func_80006F30(&D_800368D0, 0, &func_8000DF80, &D_8002C380, &D_80037A80, arg0);
        func_80007080(&D_800368D0);
        func_8000C520(sp2C);
        if (sp28 != -1) {
            func_80008380(NULL, sp28);
        }
    }
}