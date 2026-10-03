/* flags: -g0 -O2 -mips2 -G 0 -non_shared */
extern unsigned char D_8002C630;
extern void func_80014488(void);
extern void func_8001E0D4(void);
void func_80010980(void)
{
    if (D_8002C630 != 0) {
        func_80014488();
        func_8001E0D4();
        D_8002C630 = 0;
    }
}
