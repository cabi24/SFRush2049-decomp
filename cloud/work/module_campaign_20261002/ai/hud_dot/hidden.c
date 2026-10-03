/* Complete accepted Hidden implementation; original generated unused declarations omitted. */
typedef signed char s8;
typedef int s32;
extern void Input_ApplyPadConfig(void *);
s8 input_new_data_wrapper(s8 *arg0, s32 arg1) {
    if (arg1 != arg0[0x1A]) {
        arg0[0x1A] = arg1;
        Input_ApplyPadConfig(arg0);
    }
    return arg0[0x1A];
}
