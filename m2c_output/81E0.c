? func_80007080(s32);                               /* extern */
s32 func_8000C4B0();                                /* extern */
? func_8000C520(s32);                               /* extern */
? func_8000CFC4(void *);                            /* extern */
s32 func_8000D10C(void *);                          /* extern */
extern void *D_8002C3E0;

s32 func_800075E0(void *arg0, s32 arg1, s32 arg2) {
    s32 temp_s0;

    temp_s0 = func_8000C4B0();
    if (arg0->unk8 >= arg0->unk10) {
loop_1:
        if (arg2 == 1) {
            D_8002C3E0->unk10 = 8;
            func_8000CFC4(arg0 + 4);
            if (arg0->unk8 < arg0->unk10) {
                goto block_5;
            }
            goto loop_1;
        }
        func_8000C520(temp_s0);
        return -1;
    }
block_5:
    *(arg0->unk14 + (((s32) (arg0->unkC + arg0->unk8) % (s32) arg0->unk10) * 4)) = arg1;
    arg0->unk8 = (s32) (arg0->unk8 + 1);
    if (*arg0->unk0 != 0) {
        func_80007080(func_8000D10C(arg0));
    }
    func_8000C520(temp_s0);
    return 0;
}
