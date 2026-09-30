typedef signed int s32;
typedef unsigned int u32;
typedef struct { u32 w0; u32 w1; } Gfx;

extern u32 D_8012E608;
extern Gfx *D_80149438;
extern s32 D_8014A248;

void func_80086A50(s32 mode)
{
    Gfx *g;
    switch (mode) {
    case 0:
        if (D_8014A248 != 0) {
            { Gfx *q = D_80149438++; g = q; }
            g->w1 = 0x200000;
            g->w0 = 0xE3000A01;
        }
        { Gfx *q = D_80149438++; g = q; }
        g->w0 = 0xE200001C;
        g->w1 = 0x0F0A4000;
        { Gfx *q = D_80149438++; g = q; }
        g->w1 = 0xFFFCF279;
        g->w0 = 0xFCFFFFFF;
        break;
    case 1:
        if (D_8014A248 <= 0 || D_8014A248 >= 4) {
            { Gfx *q = D_80149438++; g = q; }
            g->w1 = 0;
            g->w0 = 0xE3000A01;
        }
        if ((D_8012E608 & 0x10) && (D_8012E608 & 0x20)) {
            { Gfx *q = D_80149438++; g = q; }
            g->w0 = 0xE200001C;
            g->w1 = 0x00504A70;
            { Gfx *q = D_80149438++; g = q; }
            g->w1 = 0xFF2FFFFF;
            g->w0 = 0xFC119623;
        } else if (D_8012E608 & 0x20) {
            { Gfx *q = D_80149438++; g = q; }
            g->w0 = 0xE200001C;
            g->w1 = 0x00504240;
            { Gfx *q = D_80149438++; g = q; }
            g->w1 = 0xFF2FFFFF;
            g->w0 = 0xFC119623;
        } else if (D_8012E608 & 0x10) {
            { Gfx *q = D_80149438++; g = q; }
            g->w0 = 0xE200001C;
            g->w1 = 0x00504A70;
            { Gfx *q = D_80149438++; g = q; }
            g->w1 = 0xFFFCF279;
            g->w0 = 0xFCFFFFFF;
        } else {
            { Gfx *q = D_80149438++; g = q; }
            g->w0 = 0xE200001C;
            g->w1 = 0x00504240;
            { Gfx *q = D_80149438++; g = q; }
            g->w1 = 0xFFFCF279;
            g->w0 = 0xFCFFFFFF;
        }
        break;
    case 2:
        if (D_8014A248 <= 0 || D_8014A248 >= 4) {
            { Gfx *q = D_80149438++; g = q; }
            g->w1 = 0;
            g->w0 = 0xE3000A01;
        }
        if ((D_8012E608 & 0x10) && (D_8012E608 & 0x20)) {
            { Gfx *q = D_80149438++; g = q; }
            g->w0 = 0xE200001C;
            g->w1 = 0x00504A70;
            { Gfx *q = D_80149438++; g = q; }
            g->w1 = 0xFF2FFFFF;
            g->w0 = 0xFC119623;
        } else if (D_8012E608 & 0x20) {
            { Gfx *q = D_80149438++; g = q; }
            g->w0 = 0xE200001C;
            g->w1 = 0x00504240;
            { Gfx *q = D_80149438++; g = q; }
            g->w1 = 0xFF2FFFFF;
            g->w0 = 0xFC119623;
        } else if (D_8012E608 & 0x10) {
            { Gfx *q = D_80149438++; g = q; }
            g->w0 = 0xE200001C;
            g->w1 = 0x00504A70;
            { Gfx *q = D_80149438++; g = q; }
            g->w1 = 0xFFFFF3F9;
            g->w0 = 0xFC11FE23;
        } else {
            { Gfx *q = D_80149438++; g = q; }
            g->w0 = 0xE200001C;
            g->w1 = 0x00504240;
            { Gfx *q = D_80149438++; g = q; }
            g->w1 = 0xFFFFF3F9;
            g->w0 = 0xFC11FE23;
        }
        break;
    case 3:
        if (D_8014A248 <= 0 || D_8014A248 >= 4) {
            { Gfx *q = D_80149438++; g = q; }
            g->w1 = 0;
            g->w0 = 0xE3000A01;
        }
        if ((D_8012E608 & 0x10) && (D_8012E608 & 0x20)) {
            { Gfx *q = D_80149438++; g = q; }
            g->w0 = 0xE200001C;
            g->w1 = 0x00553078;
            { Gfx *q = D_80149438++; g = q; }
            g->w1 = 0xFF2FFFFF;
            g->w0 = 0xFC119623;
        } else if (D_8012E608 & 0x20) {
            { Gfx *q = D_80149438++; g = q; }
            g->w0 = 0xE200001C;
            g->w1 = 0x0F0A7008;
            { Gfx *q = D_80149438++; g = q; }
            g->w1 = 0xFF2FFFFF;
            g->w0 = 0xFC119623;
        } else if (D_8012E608 & 0x10) {
            { Gfx *q = D_80149438++; g = q; }
            g->w0 = 0xE200001C;
            g->w1 = 0x00553078;
            { Gfx *q = D_80149438++; g = q; }
            g->w1 = 0xFFFFF3F9;
            g->w0 = 0xFC11FE23;
        } else {
            { Gfx *q = D_80149438++; g = q; }
            g->w0 = 0xE200001C;
            g->w1 = 0x0F0A7008;
            { Gfx *q = D_80149438++; g = q; }
            g->w1 = 0xFFFFF3F9;
            g->w0 = 0xFC11FE23;
        }
        break;
    case 4:
        if (D_8014A248 < 4) {
            { Gfx *q = D_80149438++; g = q; }
            g->w1 = 0x100000;
            g->w0 = 0xE3000A01;
        }
        { Gfx *q = D_80149438++; g = q; }
        g->w0 = 0xE200001C;
        g->w1 = 0x00504240;
        { Gfx *q = D_80149438++; g = q; }
        g->w1 = 0xFFFC9238;
        g->w0 = 0xFCFFABFF;
        { Gfx *q = D_80149438++; g = q; }
        g->w1 = 0x55;
        g->w0 = 0xFB000000;
        break;
    }
    D_8014A248 = mode;
}
