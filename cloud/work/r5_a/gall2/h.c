typedef signed int s32;
typedef unsigned int u32;
typedef struct { u32 w0; u32 w1; } Gfx;
extern void func_800878E0();

extern u32 D_8012E608;
extern Gfx *D_80149438;
extern s32 D_8014A248;
extern void func_80086A50();
extern void func_80086A50();

void func_800878E0(mask, b, c, d) u32 mask; s32 b, c, d;
{
    if ((D_8012E608 & mask) != mask) {
        D_8012E608 |= mask;
        if (mask & 0x4000) {
            Gfx *g = D_80149438++;
            g->w0 = 0xFCFFFFFF;
            g->w1 = 0xFFFDF6FB;
            if (D_8014A248 <= 0 || D_8014A248 >= 4) {
                g = D_80149438++;
                g->w1 = 0;
                g->w0 = 0xE3000A01;
            }
            D_8014A248 = -1;
        }
        if (mask & 1) {
            Gfx *g = D_80149438++;
            g->w0 = 0xE2001E01;
            g->w1 = 1;
        }
        if (mask & 0x10) {
            Gfx *g = D_80149438++;
            g->w0 = 0xE2001D00;
            g->w1 = 4;
            func_80086A50(D_8014A248);
        }
        if (mask & 0x20) {
            func_80086A50(D_8014A248);
        }
    }
}
void caller_h(u32 m) { func_800878E0(m); func_800878E0(0x31); }
void caller_h2(u32 m) { func_800878E0(m | 0x4000); }
