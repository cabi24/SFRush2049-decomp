/* flags: -g0 -O2 -mips2 -G 0 -non_shared */
extern volatile unsigned char D_800382CC;
extern void (*D_8003801C)(void *);
extern void *D_8003802C;
void func_80011848(void)
{
    while (D_800382CC != 0) {
    }
    D_8003801C(D_8003802C);
}
