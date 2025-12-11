s32 func_8000C4B0();                                /* extern */
? func_8000C520(s32, s32);                          /* extern */
extern void *D_8002C464;

void func_80007D20(s32 arg0) {
    s32 temp_a1;

    temp_a1 = func_8000C4B0();
    if (arg0 & 1) {
        D_8002C464->unkC = (s32) (D_8002C464->unkC | 8);
    }
    if (arg0 & 2) {
        D_8002C464->unkC = (s32) (D_8002C464->unkC & ~8);
    }
    if (arg0 & 4) {
        D_8002C464->unkC = (s32) (D_8002C464->unkC | 4);
    }
    if (arg0 & 8) {
        D_8002C464->unkC = (s32) (D_8002C464->unkC & ~4);
    }
    if (arg0 & 0x10) {
        D_8002C464->unkC = (s32) (D_8002C464->unkC | 0x10);
    }
    if (arg0 & 0x20) {
        D_8002C464->unkC = (s32) (D_8002C464->unkC & ~0x10);
    }
    if (arg0 & 0x40) {
        D_8002C464->unkC = (s32) (D_8002C464->unkC | 0x10000);
        D_8002C464->unkC = (s32) (D_8002C464->unkC & ~0x300);
    }
    if (arg0 & 0x80) {
        D_8002C464->unkC = (s32) (D_8002C464->unkC & 0xFFFEFFFF);
        D_8002C464->unkC = (s32) (D_8002C464->unkC | (D_8002C464->unk8->unk4 & 0x300));
    }
    D_8002C464->unk0 = (u16) (D_8002C464->unk0 | 8);
    func_8000C520(temp_a1, temp_a1);
}
