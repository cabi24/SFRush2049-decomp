s32 func_8000C4B0();                                /* extern */
? func_8000C520(s32, void **);                      /* extern */
extern void *D_8002C464;

void func_80006D40(void *arg0) {
    s32 temp_a0;

    temp_a0 = func_8000C4B0();
    D_8002C464->unk8 = arg0;
    D_8002C464->unk0 = 1;
    D_8002C464->unkC = (s32) D_8002C464->unk8->unk4;
    func_8000C520(temp_a0, &D_8002C464);
}
