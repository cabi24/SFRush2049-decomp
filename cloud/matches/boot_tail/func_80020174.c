/* flags: -g0 -O2 -mips2 -G 0 -non_shared */
/* Native gated wrapper; real input widths and call arity are audited. */
extern unsigned char D_8002C630;
extern void func_80014594(void);
extern void func_800145DC(void);
extern int func_8001B1D0(unsigned short, unsigned char, unsigned char);

int func_80020174(unsigned short item, unsigned char channel, unsigned char value)
{
    int result;
    result = -1;
    if (D_8002C630) {
        func_80014594();
        result = func_8001B1D0(item, channel, value);
        func_800145DC();
    }
    return result;
}
