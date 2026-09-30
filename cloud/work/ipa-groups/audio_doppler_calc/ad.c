typedef signed int s32;
typedef unsigned int u32;
typedef signed short s16;
typedef unsigned short u16;
typedef signed char s8;
typedef unsigned char u8;

/* text state at *D_801497F0 (only the bytes used here) */
typedef struct TS {
    u8 p0[2];
    u8 b2;
    u8 b3;
    u8 p4[2];
    u8 b6;
    u8 b7;
    u8 p8;
    u8 b9;
    u8 b10;
} TS;

/* glyph record, 12 bytes each in *D_80149800 */
typedef struct Glyph {
    u8 *kern;
    u8 p4;
    u8 page;
    u8 b6;
    u8 b7;
    u8 b8;
    u8 b9;
    u8 pA[2];
} Glyph;

/* texture page record in D_80149820[] */
typedef struct Tex {
    u8 p0[0x10];
    u16 w;
    u16 h;
    u8 p14[4];
    s32 img;
} Tex;

extern TS *D_801497F0;
extern Glyph *D_80149800;
extern Tex *D_80149820[];
extern s16 D_80149878[];
extern s16 D_80149D92;
extern s16 D_80149D9E;
extern s8 D_80149B60;
extern s8 D_80149B70;
extern s32 D_80149B88;
extern s32 D_80149B48;
extern s32 D_80149B08;
extern s32 D_80149B28;
extern u16 D_80149B0A;
extern u16 D_80149B2A;
extern s32 D_8012E6D0;
extern s32 D_8002AFC0;
extern s32 D_8002AFC4;
extern float D_80114748;

extern void sound_update_channel();
extern void func_8008A3E4();
extern void func_800878E0();
extern void func_8008705C();
extern void func_8008A148();
extern void object_render();
extern void func_80087110();

void audio_doppler_calc(str, len)
    u8 *str;
    u16 len;
{
    s32 x;
    s32 y;
    s32 prev;
    s32 c;
    s32 i;
    s32 step;
    s32 two;
    s32 cur;
    s32 idx;
    s32 dx;
    u8 *p;
    Glyph *g;
    Tex *t;
    TS *ts;

    sound_update_channel();
    func_8008A3E4(0, 0, D_8002AFC0 - 1, D_8002AFC4 - 1);
    func_800878E0(D_80149B88);
    func_8008705C(~D_80149B88);
    D_8012E6D0 = 0;
    if (D_80149B88 & 0x20) {
        ((u8 *) &D_80149B48)[3] = (u32) D_80114748;
    }
    func_8008A148(&D_80149B48, D_80149B08, D_80149B28, 0);
    t = D_80149820[0];
    object_render(t->img, D_80149B0A, D_80149B2A, t->w, t->h, 0, 0, t->w - 1, t->h - 1, 0, 0);
    cur = 0;
    if (str[0] == 255) {
        len -= 2;
        two = 1;
        step = 2;
        p = str + 1;
    } else {
        len -= 1;
        two = 0;
        step = 1;
        p = str;
    }
    x = D_80149D92;
    y = D_80149D9E;
    prev = -1;
    for (i = 0; i < (s32) len; i += step) {
        if (two) {
            c = (p[i] << 8) | p[i + 1];
        } else {
            c = p[i];
        }
        if (c == 10) {
            ts = D_801497F0;
            x = D_80149D92;
            c = -1;
            y = y + ts->b2 + ts->b3 + D_80149B60;
        } else if ((c == 32) || (c >= 256) || ((idx = D_80149878[c]) < 0)) {
            ts = D_801497F0;
            c = -1;
            if (ts->b9 != 0) {
                x += ts->b6 + D_80149B70;
            } else {
                x += ts->b7;
            }
        } else {
            if (prev > 0) {
                ts = D_801497F0;
                if ((ts->b10 != 0) && (ts->b9 == 0)) {
                    x -= D_80149800[idx].kern[prev];
                }
            }
            g = &D_80149800[idx];
            if (cur != g->page) {
                cur = g->page;
                t = D_80149820[cur];
                object_render(t->img, D_80149B0A, D_80149B2A, t->w, t->h, 0, 0, t->w - 1, t->h - 1, 0, 0);
            }
            dx = 0;
            if (D_801497F0->b9 != 0) {
                dx = (D_801497F0->b6 - g->b8 + g->b6 - 1) / 2;
            }
            func_80087110(dx + x, y, g->b8 + (dx + x) - g->b6, g->b9 + y - g->b7, g->b6, g->b7);
            if (D_801497F0->b9 != 0) {
                dx = D_801497F0->b6;
            } else {
                dx = g->b8 - g->b6 + 1;
            }
            x = x + dx + D_80149B70;
        }
        prev = c;
    }
}

void caller_a(u8 *s, u16 n) { audio_doppler_calc(s, n); audio_doppler_calc(s, n + 1); }
void caller_b(u8 *s, u16 n) { audio_doppler_calc(s, n); }
