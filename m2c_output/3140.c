s32 func_80004AFC(s32, s32);                        /* extern */
s32 func_80006814(s32, s32, s32);                   /* extern */
? func_80006A00(? *, ? *, ?);                       /* extern */
s32 func_80007270(? *, ? *, ?);                     /* extern */
? func_800075E0(? *, ?, ?);                         /* extern */
extern s8 D_8002B030;
extern ? D_8002F190;
extern ? D_8002F1A8;

void func_80002540(void) {
    D_8002B030 = 1;
    func_80006A00(&D_8002F190, &D_8002F1A8, 1);
    func_800075E0(&D_8002F190, 0, 0);
}

s32 func_80002598(s32 arg0) {
    ? sp1C;

    if (D_8002B030 == 0) {
        func_80002540();
    }
    if (arg0 != 0) {
        func_80007270(&D_8002F190, &sp1C, 1);
        goto block_6;
    }
    if (func_80007270(&D_8002F190, &sp1C, 0) == -1) {
        return 0;
    }
block_6:
    return 1;
}

void func_8000262C(void) {
    func_800075E0(&D_8002F190, 0, 0);
}

s32 func_80002660(s32 arg0, s32 arg1) {
    s32 sp1C;
    s32 temp_v0;

    if (func_80002598(1) == 0) {
        return 0;
    }
    temp_v0 = func_80004AFC(arg0, arg1);
    sp1C = temp_v0;
    func_8000262C();
    return temp_v0;
}

s32 func_800026C0(s32 arg0, s32 arg1, s32 arg2) {
    s32 sp1C;
    s32 temp_v0;

    if (func_80002598(1) == 0) {
        return 0;
    }
    temp_v0 = func_80006814(arg0, arg1, arg2);
    sp1C = temp_v0;
    func_8000262C();
    return temp_v0;
}
