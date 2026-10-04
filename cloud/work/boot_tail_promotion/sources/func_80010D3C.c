/* flags: -g0 -O2 -mips2 -G 0 -non_shared */
/* Adapted from cloud/matches/boot_tail/func_80010D3C.c: calls renamed to their canonical
 * symbol_addrs names (func_8000BF00 -> osAiSetFrequency); no other change. */
extern unsigned int osAiSetFrequency(unsigned int);
extern unsigned int D_8003828C;
void func_80010D3C(unsigned int *frequency)
{
    D_8003828C = osAiSetFrequency(*frequency);
    *frequency = D_8003828C;
}
