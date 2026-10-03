/* flags: -g0 -O2 -mips2 -G 0 -non_shared */
extern void *D_800382F0;
extern void (*D_8003801C)(void *);
extern void func_80011074(void);
void func_800110C4(void)
{
    func_80011074();
    if (D_800382F0 != 0) {
        D_8003801C(D_800382F0);
    }
}
