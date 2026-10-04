/* flags: -g0 -O2 -mips2 -G 0 -non_shared */
extern unsigned char D_8002C630;
extern void func_800144F0(void);
extern void func_8001E0D4(void);
void func_800109C0(void)
{
    if (D_8002C630 != 0) {
        func_800144F0();
        func_8001E0D4();
        D_8002C630 = 0;
    }
}
