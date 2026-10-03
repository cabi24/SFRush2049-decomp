/* flags: -g0 -O2 -mips2 -G 0 -non_shared */
extern unsigned short D_80038028;
extern volatile unsigned char D_800382CC;
extern void *D_8003802C;
extern unsigned char D_800382B0[];
extern void (*D_80038010)(void *, unsigned short, void *);
void func_80011D24(void)
{
    if (D_80038028 != 0) {
        D_800382CC = 1;
        D_80038010(D_8003802C, D_80038028, D_800382B0);
    }
}
