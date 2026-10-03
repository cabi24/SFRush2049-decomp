/* flags: -g0 -O2 -mips2 -G 0 -non_shared */
extern void *D_800382E8;
extern void (*D_8003801C)(void *);
extern void func_800118C0(void);
void func_80011074(void)
{
    if (D_800382E8 != 0) {
        func_800118C0();
        D_8003801C(D_800382E8);
        D_800382E8 = 0;
    }
}
