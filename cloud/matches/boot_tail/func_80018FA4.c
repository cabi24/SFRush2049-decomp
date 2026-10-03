/* flags: -g0 -O2 -mips2 -G 0 -non_shared */
/* Native gated service wrapper; genuine scalar and pointer inputs audited. */
extern unsigned char D_8002C630;
extern void func_80014594(void);
extern void func_800145DC(void);
extern void func_80018F20(unsigned int, unsigned short);
void func_80018FA4(unsigned int identifier, unsigned short value)
{
    if (D_8002C630) {
        func_80014594();
        func_80018F20(identifier, value);
        func_800145DC();
    }
}
