/* flags: -g0 -O2 -mips2 -G 0 -non_shared */
/* Native reconstruction; original middleware names and full types are unknown. */
extern unsigned int D_8004FA20[];
unsigned int func_80019AA8(unsigned char *state)
{
    return D_8004FA20[state[75] == 255 ? 8 : state[75]];
}
