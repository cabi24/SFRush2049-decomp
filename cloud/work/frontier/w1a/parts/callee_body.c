
extern u32 D_8012E608;
extern Gfx *D_80149438;
extern s32 D_8014A248;

void func_80086A50(s32 mode)
{
    switch (mode) {
    case 0:
        if (D_8014A248 != 0) {
            gDPSetCycleType(D_80149438++, 0x200000);
        }
        gDPSetRenderModeRaw(D_80149438++, 0x0F0A4000);
        gDPSetCombine(D_80149438++, 0xFFFFFF, 0xFFFCF279);
        break;
    case 1:
        if (D_8014A248 <= 0 || D_8014A248 >= 4) {
            gDPSetCycleType(D_80149438++, 0);
        }
        if ((D_8012E608 & 0x10) && (D_8012E608 & 0x20)) {
            gDPSetRenderModeRaw(D_80149438++, 0x00504A70);
            gDPSetCombine(D_80149438++, 0x119623, 0xFF2FFFFF);
        }
        else if (D_8012E608 & 0x20) {
            gDPSetRenderModeRaw(D_80149438++, 0x00504240);
            gDPSetCombine(D_80149438++, 0x119623, 0xFF2FFFFF);
        }
        else if (D_8012E608 & 0x10) {
            gDPSetRenderModeRaw(D_80149438++, 0x00504A70);
            gDPSetCombine(D_80149438++, 0xFFFFFF, 0xFFFCF279);
        }
        else {
            gDPSetRenderModeRaw(D_80149438++, 0x00504240);
            gDPSetCombine(D_80149438++, 0xFFFFFF, 0xFFFCF279);
        }
        break;
    case 2:
        if (D_8014A248 <= 0 || D_8014A248 >= 4) {
            gDPSetCycleType(D_80149438++, 0);
        }
        if ((D_8012E608 & 0x10) && (D_8012E608 & 0x20)) {
            gDPSetRenderModeRaw(D_80149438++, 0x00504A70);
            gDPSetCombine(D_80149438++, 0x119623, 0xFF2FFFFF);
        }
        else if (D_8012E608 & 0x20) {
            gDPSetRenderModeRaw(D_80149438++, 0x00504240);
            gDPSetCombine(D_80149438++, 0x119623, 0xFF2FFFFF);
        }
        else if (D_8012E608 & 0x10) {
            gDPSetRenderModeRaw(D_80149438++, 0x00504A70);
            gDPSetCombine(D_80149438++, 0x11FE23, 0xFFFFF3F9);
        }
        else {
            gDPSetRenderModeRaw(D_80149438++, 0x00504240);
            gDPSetCombine(D_80149438++, 0x11FE23, 0xFFFFF3F9);
        }
        break;
    case 3:
        if (D_8014A248 <= 0 || D_8014A248 >= 4) {
            gDPSetCycleType(D_80149438++, 0);
        }
        if ((D_8012E608 & 0x10) && (D_8012E608 & 0x20)) {
            gDPSetRenderModeRaw(D_80149438++, 0x00553078);
            gDPSetCombine(D_80149438++, 0x119623, 0xFF2FFFFF);
        }
        else if (D_8012E608 & 0x20) {
            gDPSetRenderModeRaw(D_80149438++, 0x0F0A7008);
            gDPSetCombine(D_80149438++, 0x119623, 0xFF2FFFFF);
        }
        else if (D_8012E608 & 0x10) {
            gDPSetRenderModeRaw(D_80149438++, 0x00553078);
            gDPSetCombine(D_80149438++, 0x11FE23, 0xFFFFF3F9);
        }
        else {
            gDPSetRenderModeRaw(D_80149438++, 0x0F0A7008);
            gDPSetCombine(D_80149438++, 0x11FE23, 0xFFFFF3F9);
        }
        break;
    case 4:
        if (D_8014A248 < 4) {
            gDPSetCycleType(D_80149438++, 0x100000);
        }
        gDPSetRenderModeRaw(D_80149438++, 0x00504240);
        gDPSetCombine(D_80149438++, 0xFFABFF, 0xFFFC9238);
        gDPSetEnvColor(D_80149438++, 0, 0, 0, 0x55);
        break;
    }
    D_8014A248 = mode;
}
