typedef unsigned char u8;
typedef unsigned short u16;
typedef signed int s32;
typedef unsigned int u32;
typedef struct { u32 w0; u32 w1; } Gfx;

extern u32 D_8012E608;
extern Gfx *D_80149438;
extern s32 D_8014A248;
extern u8 D_8011EACC[];
extern s32 D_8012E6D0;
extern u16 D_8012E67A;
extern void func_80086A50();
extern void func_8008A148();
extern void func_8008A644(u16);
extern void func_800878E0(u32);

void func_8008A38C(u16 arg0)
{
    if (D_8011EACC[3] != arg0) {
        D_8011EACC[3] = arg0;
        D_8012E6D0 = 0;
    }
    func_8008A148(D_8011EACC, 4, 1, 0);
    func_800878E0(32);
}

void func_8008A644(u16 arg0)
{
    if (D_8012E67A != arg0) {
        Gfx *g = D_80149438++;
        g->w1 = arg0 << 16;
        g->w0 = 0xEE000000;
        D_8012E67A = arg0;
    }
    func_800878E0(16);
}

void func_800878E0(u32 mask)
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

/* stand-in callers (real: object_render, func_8008A46C, Input_ProcessGameplayPad, sound_init, audio_doppler_calc) */
void caller_a(u32 m) { func_800878E0(m); func_800878E0(0x4000); }
void caller_b(u32 m) { func_800878E0(m | 1); }

extern void ext_use();
/* keeps a3 live across the call: real callers hold values in a3 */
void caller_c(u16 a, s32 b, s32 c, s32 d) { func_8008A644(a); ext_use(d); }
void caller_d(u16 a, s32 b, s32 c, s32 d) { func_8008A644(a); func_8008A644(d); ext_use(d); }
