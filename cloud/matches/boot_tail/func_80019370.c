/* flags: -g0 -O2 -mips2 -G 0 -non_shared */
/* Native gated service wrapper; genuine scalar and pointer inputs audited. */
extern unsigned char D_8002C630;
extern void func_80014594(void);
extern void func_800145DC(void);
extern void func_80019194(unsigned char, unsigned short, unsigned int, unsigned char);
void func_80019370(unsigned char value, unsigned short duration, unsigned int identifier, unsigned char mode)
{
    if (D_8002C630) {
        func_80014594();
        func_80019194(value, duration, identifier, mode);
        func_800145DC();
    }
}
