/* flags: -g0 -O2 -mips2 -G 0 -non_shared */
/* COMPLETE-NONMATCH: see README.md and verification.json. */
extern unsigned char D_8002C630;
unsigned char func_8001D1C0(unsigned int *state)
{
    if (D_8002C630) return (state[2] & 0x10000) != 0;
    return 0;
}
