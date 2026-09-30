/*@@HDR 0 3086@@*/
u8 *func_800A4770(u8 *arg0, u8 *arg1);
/*@@HDR 3087 3413@@*/

/*@@HDR 3414 3697@@*/
#endif

s8 input_new_data_wrapper(s8 *arg0, s32 arg1) {
    if (arg1 != arg0[0x1A]) {
        arg0[0x1A] = arg1;
        Input_ApplyPadConfig(arg0);
    }
    return arg0[0x1A];
}
