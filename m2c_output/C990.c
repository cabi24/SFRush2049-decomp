? func_8000C050(s32, void *);                       /* extern */
s32 func_8000C4B0();                                /* extern */
? func_8000C520(s32);                               /* extern */
? func_8000CFC4(?);                                 /* extern */
extern void *D_8002C3E0;

void func_8000BD90(void *arg0) {
    s32 temp_s0;
    u16 var_s1;

    temp_s0 = func_8000C4B0();
    if (arg0 == NULL) {
        var_s1 = 4;
    } else {
        var_s1 = arg0->unk10;
    }
    switch (var_s1) {                               /* irregular */
    case 4:
        D_8002C3E0->unk10 = 1;
        func_8000CFC4(0);
        break;
    case 2:
    case 8:
        arg0->unk10 = 1U;
        func_8000C050(arg0->unk8, arg0);
        break;
    }
    func_8000C520(temp_s0);
}

s32 func_8000BE50(void *arg0) {
    void *var_a0;

    var_a0 = arg0;
    if (var_a0 == NULL) {
        var_a0 = D_8002C3E0;
    }
    return var_a0->unk14;
}
