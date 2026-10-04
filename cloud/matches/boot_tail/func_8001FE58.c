/* flags: -g0 -O2 -mips2 -G 0 -non_shared */
/* Native gated API wrapper; parameter widths follow actual callee entries. */
extern unsigned char D_8002C630;
extern void func_80014594(void);
extern void func_800145DC(void);
extern int func_8001B8C4(unsigned int);

int func_8001FE58(unsigned int identifier)
{
    int result;
    result = -1;
    if (D_8002C630) {
        func_80014594();
        result = func_8001B8C4(identifier);
        func_800145DC();
    }
    return result;
}
