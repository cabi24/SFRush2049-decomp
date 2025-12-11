? func_8000DC10();                                  /* extern */
? func_8000DC54();                                  /* extern */
s32 func_8000DC80(s32, s32);                        /* extern */

s32 func_800081D0(s32 arg0, s32 arg1) {
    s32 sp1C;
    s32 temp_v0;

    func_8000DC10();
    temp_v0 = func_8000DC80(arg0, arg1);
    sp1C = temp_v0;
    func_8000DC54();
    return temp_v0;
}
