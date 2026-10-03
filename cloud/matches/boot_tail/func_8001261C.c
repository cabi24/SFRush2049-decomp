/* flags: -g0 -O2 -mips2 -G 0 -non_shared */
extern void (*D_8003801C)(void *);
extern void *D_80038350;
extern void *D_8003834C;
void func_8001261C(void)
{
    D_8003801C(D_80038350);
    D_8003801C(D_8003834C);
}
