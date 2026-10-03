/* flags: -g0 -O2 -mips2 -G 0 -non_shared */
extern void (*D_8003801C)(void *);
extern void *D_8003833C;
extern void *D_80038338;
void func_800121BC(void)
{
    D_8003801C(D_8003833C);
    D_8003801C(D_80038338);
}
