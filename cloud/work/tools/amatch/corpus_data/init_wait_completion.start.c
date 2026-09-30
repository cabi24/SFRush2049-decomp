/*@@HDR 1 3086@@*/
u8 *func_800A4770(u8 *arg0, u8 *arg1);
/*@@HDR 3087 3411@@*/

/*@@HDR 3412 3697@@*/
#endif

M2C_UNK func_80020274();                            /* extern */
s32 func_800202c4();                                /* extern */

void init_wait_completion(void) {
    wheel_params_set();
    func_80096238();
    func_80020274();
    if (func_800202c4() == 0) {
        do {

        } while (func_800202c4() == 0);
    }
}
