
extern u32 D_8012E608;
extern Gfx *D_80149438;
extern s32 D_8014A248;
extern void func_80086A50();

void func_8008705C(u32 mask)
{
    if (D_8012E608 & mask) {
        D_8012E608 &= ~mask;
        if (mask & 1) {
            gDPSetAlphaCompare(D_80149438++, G_AC_NONE);
        }
        if (mask & 0x10) {
            gDPSetDepthSource(D_80149438++, G_ZS_PIXEL);
            func_80086A50(D_8014A248);
        }
        if (mask & 0x20) {
            func_80086A50(D_8014A248);
        }
    }
}

void func_800878E0(u32 mask)
{
    if ((D_8012E608 & mask) != mask) {
        D_8012E608 |= mask;
        if (mask & 0x4000) {
            gDPSetCombine(D_80149438++, 0xFFFFFF, 0xFFFDF6FB);
            if (D_8014A248 <= 0 || D_8014A248 >= 4) {
                gDPSetCycleType(D_80149438++, G_CYC_1CYCLE);
            }
            D_8014A248 = -1;
        }
        if (mask & 1) {
            gDPSetAlphaCompare(D_80149438++, G_AC_THRESHOLD);
        }
        if (mask & 0x10) {
            gDPSetDepthSource(D_80149438++, G_ZS_PRIM);
            func_80086A50(D_8014A248);
        }
        if (mask & 0x20) {
            func_80086A50(D_8014A248);
        }
    }
}
