/* flags: -g0 -O2 -mips2 -G 0 -non_shared */
typedef unsigned char u8;
extern u8 D_8002C630;
extern u8 D_8004BE94;
extern u8 D_8004FA18;
extern int func_800146AC(void);
extern u8 func_8001467C(int);
u8 func_800202C4(void)
{
    u8 active;
    int i;
    active = 0;
    if (D_8002C630) {
        D_8004BE94 = 1;
        if (func_800146AC()) {
            active = 1;
        } else {
            for (i = 0; i < D_8004FA18; i++) {
                active |= func_8001467C(i);
            }
        }
        D_8004BE94 = 0;
    }
    return active == 0;
}
