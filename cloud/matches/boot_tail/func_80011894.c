/* flags: -g0 -O2 -mips2 -G 0 -non_shared */
extern unsigned int D_8003802C;
extern unsigned int D_80038038;
extern unsigned int D_8003803C;
extern unsigned short D_80038028;
void func_80011894(void)
{
    unsigned int value;
    value = D_8003802C;
    D_8003803C = value & ~15U;
    D_80038038 = value;
    D_80038028 = 0;
}
