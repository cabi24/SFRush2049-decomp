s32 func_80007440(s32, void *, ?);                  /* extern */
s32 func_800075E0(s32, void *, ?);                  /* extern */
s32 func_8000E390();                                /* extern */
extern s32 D_8002C380;

s32 func_80008630(void *arg0, s8 arg1, s32 arg2, s32 arg3, s32 arg4, s32 arg5, s32 arg6) {
    s32 var_v0;

    if (D_8002C380 == 0) {
        return -1;
    }
    if (arg2 == 0) {
        arg0->unk0 = 0xB;
    } else {
        arg0->unk0 = 0xC;
    }
    arg0->unk2 = arg1;
    arg0->unk4 = arg6;
    arg0->unkC = arg3;
    arg0->unk8 = arg4;
    arg0->unk14 = 0;
    arg0->unk10 = arg5;
    if (arg1 == 1) {
        var_v0 = func_80007440(func_8000E390(), arg0, 0);
    } else {
        var_v0 = func_800075E0(func_8000E390(), arg0, 0);
    }
    return var_v0;
}
