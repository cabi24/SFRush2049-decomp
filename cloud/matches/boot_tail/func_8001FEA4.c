/* flags: -g0 -O2 -mips2 -G 0 -non_shared */
/* Native gated API wrapper; parameter widths follow actual callee entries. */
extern unsigned char D_8002C630;
extern void func_80014594(void);
extern void func_800145DC(void);
extern int func_8001B7C0(unsigned int, unsigned char);

int func_8001FEA4(unsigned int identifier, unsigned char value)
{
    int result;
    result = -1;
    if (D_8002C630) {
        func_80014594();
        result = func_8001B7C0(identifier, value);
        func_800145DC();
    }
    return result;
}
