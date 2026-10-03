/* flags: -g0 -O2 -mips2 -G 0 -non_shared */
/* Native gated wrapper; real input widths and call arity are audited. */
extern unsigned char D_8002C630;
extern void func_80014594(void);
extern void func_800145DC(void);
extern void func_80018FEC(unsigned int);

void func_8001906C(unsigned int identifier)
{
    if (D_8002C630) {
        func_80014594();
        func_80018FEC(identifier);
        func_800145DC();
    }
}
