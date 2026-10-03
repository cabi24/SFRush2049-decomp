typedef signed int s32;
typedef unsigned int u32;
typedef struct { u32 w0, w1; } Gfx;
extern u32 D_8012E608;
extern Gfx *D_80149438;
extern s32 D_8014A248;
extern void func_80086A50(s32);

void func_800878E0(u32 flags)
{
    Gfx *g;
    if ((D_8012E608 & flags) != flags) {
        D_8012E608 |= flags;
        if (flags & 0x4000) {
            g = D_80149438++;
            g->w0 = 0xFCFFFFFF; g->w1 = 0xFFFDF6FB;
            if (D_8014A248 <= 0 || D_8014A248 >= 4) {
                g = D_80149438++;
                g->w0 = 0xE3000A01; g->w1 = 0;
            }
            D_8014A248 = -1;
        }
        if (flags & 1) {
            g = D_80149438++;
            g->w0 = 0xE2001E01; g->w1 = 1;
        }
        if (flags & 0x10) {
            g = D_80149438++;
            g->w0 = 0xE2001D00; g->w1 = 4;
            func_80086A50(D_8014A248);
        }
        if (flags & 0x20) func_80086A50(D_8014A248);
    }
}
