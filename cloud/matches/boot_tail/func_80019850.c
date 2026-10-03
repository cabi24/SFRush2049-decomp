/* flags: -g0 -O2 -mips2 -G 0 -non_shared */
/* Native gated service wrapper; genuine scalar and pointer inputs audited. */
extern unsigned char D_8002C630;
extern void func_80014594(void);
extern void func_800145DC(void);
extern void func_80019490(void *, unsigned int *, unsigned char);
void func_80019850(void *state, unsigned int *result)
{
    if (D_8002C630) {
        func_80014594();
        func_80019490(state, result, 0);
        func_800145DC();
    }
}
