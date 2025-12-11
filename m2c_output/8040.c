? func_80007080(s32);                               /* extern */
s32 func_8000C4B0();                                /* extern */
? func_8000C520(s32, void **);                      /* extern */
? func_8000CFC4(void *);                            /* extern */
s32 func_8000D10C(void *);                          /* extern */
extern void *D_8002C3E0;
extern void *D_8002C464;

s32 func_80007440(void *arg0, s32 arg1, s32 arg2) {
    s32 temp_s0;
    s32 temp_t7;

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
    temp_t7 = arg0->unk10;
    arg0->unkC = (s32) ((s32) ((arg0->unkC + temp_t7) - 1) % temp_t7);
    *(arg0->unk14 + (arg0->unkC * 4)) = arg1;
    arg0->unk8 = (s32) (arg0->unk8 + 1);
    if (*arg0->unk0 != 0) {
        func_80007080(func_8000D10C(arg0));
    }
    func_8000C520(temp_s0);
    return 0;
}

void func_80007590(s32 arg0) {
    s32 temp_a0;

    temp_a0 = func_8000C4B0();
    D_8002C464->unk4 = arg0;
    D_8002C464->unk0 = (u16) (D_8002C464->unk0 | 0x10);
    func_8000C520(temp_a0, &D_8002C464);
}
