/* flags: -g0 -O2 -mips2 -G 0 -non_shared */
/* Native gated wrapper; real input widths and call arity are audited. */
extern unsigned char D_8002C630;
extern void func_80014594(void);
extern void func_800145DC(void);
extern void func_8001B9F8(unsigned char, unsigned short, unsigned char, unsigned char, unsigned int);

void func_8002043C(unsigned char channel, unsigned short duration, unsigned char mode)
{
    if (D_8002C630) {
        func_80014594();
        func_8001B9F8(channel, duration, mode, 0, 0);
        func_800145DC();
    }
}
