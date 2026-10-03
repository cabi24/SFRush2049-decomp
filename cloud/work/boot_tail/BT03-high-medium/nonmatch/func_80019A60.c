/* flags: -g0 -O2 -mips2 -G 0 -non_shared */
/* COMPLETE-NONMATCH: see README.md and verification.json. */
extern unsigned int D_8004FA20[];
void func_80019A60(unsigned int value, unsigned char index)
{
    if (index == 255) index = 8;
    D_8004FA20[index] = (value * 8) * 1536 / 240;
}
