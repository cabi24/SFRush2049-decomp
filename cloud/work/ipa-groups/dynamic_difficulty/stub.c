typedef signed int s32;

extern s32 D_80149B08;
extern s32 D_80149B28;

/* Context stand-in for func_80096288. Retail is `beqz a2,L; nop; L: jr ra; nop`
 * (an empty debug check that IDO -O3 will not reproduce: an empty `if` is deleted
 * and the calls to it disappear). The store keeps the call alive; the dead switch
 * keeps umerge from inlining it. Register footprint is what matters here. */
void func_80096288(s32 a, s32 b, s32 c)
{
    if (0) { switch (c) { case 0: D_80149B08 = 1; break; case 1: D_80149B08 = 5; break; case 2: D_80149B08 = 7; break; case 3: D_80149B08 = 9; break; } }
    if (c) {
        D_80149B28 = 1;
    }
}
