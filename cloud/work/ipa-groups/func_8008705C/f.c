typedef signed int s32;
typedef unsigned int u32;
typedef struct { u32 w0; u32 w1; } Gfx;

extern u32 D_8012E608;
extern Gfx *D_80149438;
extern s32 D_8014A248;
extern s32 D_8012F001, D_8012F002, D_8012F003, D_8012F004, D_8012F005;
extern void func_80086A50();
extern void func_8008705C(u32);

void func_8008705C(u32 mask)
{
    if (D_8012E608 & mask) {
        D_8012E608 &= ~mask;
        if (mask & 1) {
            Gfx *g = D_80149438++;
            g->w1 = 0;
            g->w0 = 0xE2001E01;
        }
        if (mask & 0x10) {
            Gfx *g = D_80149438++;
            g->w1 = 0;
            g->w0 = 0xE2001D00;
            func_80086A50(D_8014A248);
        }
        if (mask & 0x20) {
            func_80086A50(D_8014A248);
        }
    }
}

/* stand-in callers (real: func_8008A46C, Input_ProcessGameplayPad, audio_doppler_calc) */
void caller_a(u32 m) { func_8008705C(m); func_8008705C(0x31); }
void caller_b(u32 m) { func_8008705C(m | 1); }

