/* flags: -g0 -O2 -mips2 -G 0 -non_shared */
extern volatile unsigned char D_800382CD;
extern void (*D_80038004)(void);
extern void func_80010110(void);
void func_800118C0(void)
{
    if (D_800382CD != 0) {
        D_80038004();
        func_80010110();
        D_800382CD = 0;
    }
}
