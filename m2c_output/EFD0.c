? func_8000C294(s32, u32);                          /* extern */
s32 func_8000C308(void *);                          /* extern */
extern s32 *D_8002C3F0;

s32 func_8000E3D0(void *arg0, s32 arg2, s32 arg3, s32 arg4, s32 arg5, s32 arg6, s32 arg7) {
    u32 sp1C;
    s32 sp18;
    s32 temp_ret;

    arg0->unk0 = 0;
    arg0->unk4 = 0;
    arg0->unkC = arg5;
    arg0->unk8 = arg4;
    if ((arg2 != 0) || (arg3 != 0)) {
        arg0->unk10 = arg2;
        arg0->unk14 = arg3;
    } else {
        arg0->unk10 = arg4;
        arg0->unk14 = arg5;
    }
    arg0->unk18 = arg6;
    arg0->unk1C = arg7;
    temp_ret = func_8000C308(arg0);
    sp18 = temp_ret;
    sp1C = (u32) (u64) temp_ret;
    if (*D_8002C3F0 == arg0) {
        func_8000C294(sp18, sp1C);
    }
    return 0;
}
