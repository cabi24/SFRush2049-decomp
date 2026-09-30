/*@@HDR 1 3086@@*/
u8 *func_800A4770(u8 *arg0, u8 *arg1);
/*@@HDR 3087 3413@@*/

/*@@HDR 3414 3697@@*/
#endif

s8 input_new_data_wrapper(void *arg0, s8 arg1) {
    s8 var_v1;

    var_v1 = M2C_FIELD(arg0, s8 *, 0x1A);
    if (arg1 != var_v1) {
        M2C_FIELD(arg0, s8 *, 0x1A) = arg1;
        Input_ApplyPadConfig(arg0);
        var_v1 = M2C_FIELD(arg0, s8 *, 0x1A);
    }
    return var_v1;
}
