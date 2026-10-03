/* flags: -g0 -O2 -mips2 -G 0 -non_shared */
extern unsigned int func_8000BF00(unsigned int);
extern unsigned int D_8003828C;
void func_80010D3C(unsigned int *frequency)
{
    D_8003828C = func_8000BF00(*frequency);
    *frequency = D_8003828C;
}
