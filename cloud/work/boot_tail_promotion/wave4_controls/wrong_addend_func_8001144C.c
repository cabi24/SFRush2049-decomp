/* flags: -g0 -O2 -mips2 -G 0 -non_shared */
extern void (*D_8003801C)(void *);
extern unsigned short *D_800382D8[2];
extern void *D_80038298;
extern void *D_800382E4;
void func_8001144C(void)
{
    D_8003801C(D_800382D8[0]);
    D_8003801C(D_800382D8[2]);
    D_8003801C(D_80038298);
    D_8003801C(D_800382E4);
}
