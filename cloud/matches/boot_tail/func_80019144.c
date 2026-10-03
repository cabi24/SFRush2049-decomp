/* flags: -g0 -O2 -mips2 -G 0 -non_shared */
/* Native gated service wrapper; genuine scalar and pointer inputs audited. */
extern unsigned char D_8002C630;
extern void func_80014594(void);
extern void func_800145DC(void);
extern void func_800190AC(unsigned int, unsigned int, unsigned int);
void func_80019144(unsigned int identifier, unsigned int first, unsigned int second)
{
    if (D_8002C630) {
        func_80014594();
        func_800190AC(identifier, first, second);
        func_800145DC();
    }
}
