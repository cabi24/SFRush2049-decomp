/* flags: -g0 -O2 -mips2 -G 0 -non_shared */
/* COMPLETE-NONMATCH: see README.md and verification.json. */
typedef unsigned int u32;
extern u32 *D_8004BE80;
void func_80018AEC(void)
{
    u32 value;
    if (D_8004BE80[986]) {
        value = D_8004BE80[70] + D_8004BE80[988];
        D_8004BE80[988] = value & 0xFFFF;
        value >>= 16;
        D_8004BE80[989] = D_8004BE80[989] + value + D_8004BE80[71];
    }
}
