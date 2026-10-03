/* flags: -g0 -O2 -mips2 -G 0 -non_shared */
/* COMPLETE-NONMATCH: see README.md and verification.json. */
extern short D_8004BE98[];
short func_80020200(unsigned char value)
{
    return D_8004BE98[value & 15];
}
