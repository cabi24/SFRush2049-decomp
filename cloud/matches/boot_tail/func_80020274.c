/* flags: -g0 -O2 -mips2 -G 0 -non_shared */
/* Native state-service reconstruction; actual input and data accesses audited. */
extern unsigned char D_8002C630;
extern void func_80014594(void);
extern void func_800145DC(void);
extern void func_80018E6C(void);
extern void func_8001D578(void);
extern void func_8001FAE4(unsigned char);
void func_80020274(void)
{
    if (D_8002C630) {
        func_80014594();
        func_80018E6C();
        func_8001D578();
        func_8001FAE4(0);
        func_800145DC();
    }
}
