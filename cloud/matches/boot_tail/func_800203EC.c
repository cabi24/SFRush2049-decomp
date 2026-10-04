/* flags: -g0 -O2 -mips2 -G 0 -non_shared */
/* Native gated wrapper; real input widths and call arity are audited. */
extern unsigned char D_8002C630;
extern void func_80014594(void);
extern void func_800145DC(void);
extern void func_8001BE14(unsigned char, unsigned short, unsigned char);

void func_800203EC(unsigned char channel, unsigned short duration, unsigned char mode)
{
    if (D_8002C630) {
        func_80014594();
        func_8001BE14(channel, duration, mode);
        func_800145DC();
    }
}
