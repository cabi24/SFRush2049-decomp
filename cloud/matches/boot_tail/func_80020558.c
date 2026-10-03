/* flags: -g0 -O2 -mips2 -G 0 -non_shared */
/* Native state-service reconstruction; actual input and data accesses audited. */
extern unsigned char D_8002C630;
extern void func_80014594(void);
extern void func_800145DC(void);
extern void func_80014BF8(void);
void func_80020558(void)
{
    if (D_8002C630) {
        func_80014594();
        func_80014BF8();
        func_800145DC();
    }
}
