/* flags: -g0 -O3 -mips2 -G 0 -non_shared */
/*
 * lap_complete (0x8010761C, 1,232 B) -- historical label: draws the race clock (D_801543CC) as "mm:ss.hh"
 * with a shadow pass, in attract-free play (state_word_a bit 22, not bit 3) when D_8015723C is set.
 * Digits are formatted by hand (/600000 %6, /60000 %10, ':', /10000 %6, /1000 %10, '.', /100 %10, /10 %10, %10);
 * font 1 or 2 by viewport count (font_set, inlined static of credits_scroll_grp gr3_c.c), digit/colon widths
 * from camera_shake_update('8'/':') with object_byte9_set(0) around the second; position from
 * D_80115C88[count-1]; each glyph drawn by state_utility(x, y, ch) after dispatch_handler(0) shadow ('8') /
 * dispatch_handler(1); render_helper(0) / func_800B669C(1,3) before, func_800B669C(0,3) / render_helper(-1) after.
 * STATE: draft. unit 288/308 words (rows words 111 ops 34 norm 34), frame 184/176.
 * Residual class: shaping. Retail loads y with lw + explicit narrowing (ours folds to lh -2), keeps no register
 * for &D_80151AD0 (ours s5, one more s-reg), and has an 8-byte larger frame (one more inlined call; an inlined
 * draw helper with an s16 y parameter overshoots to 200). race_finish shares this layout.
 */
typedef signed char s8;
typedef unsigned char u8;
typedef signed short s16;
typedef unsigned short u16;
typedef signed int s32;
typedef unsigned int u32;
typedef float f32;
typedef struct { u32 opaque[6]; } OSMesgQueue;
typedef struct { u32 first, second; } Pair;
typedef struct { s32 x, y; } Pos;
extern OSMesgQueue D_801461D0;
extern s32 state_word_a;
extern s8 D_8015723C;
extern f32 D_801543CC;
extern s16 D_80151AD0;
extern Pair D_80118E20;
extern Pos D_80115C88[];
extern s32 osRecvMesg(OSMesgQueue *, void **, s32);
extern s32 osSendMesg(OSMesgQueue *, void *, s32);
extern s32 slot_state_setup(s32 selection);
extern void render_helper(f32);
extern s32 camera_shake_update(u16 id);
extern s8 object_byte9_set(s8 value);
extern void dispatch_handler(s32);
extern void state_utility(s16, s32, void *);

static void gfx_lock(void)
{
    osRecvMesg(&D_801461D0, (void *)0, 1);
}

static void gfx_unlock(void)
{
    osSendMesg(&D_801461D0, (void *)0, 0);
}

static s32 font_set(s32 font)
{
    s32 old;

    gfx_lock();
    old = slot_state_setup(font);
    gfx_unlock();
    return old;
}

extern void func_800B669C(u32 first, u32 second);

s32 lap_complete(s32 arg0)
{
    char text[12];
    char ch[2];
    s32 ms;
    s32 w;
    s32 w2;
    s8 old;
    s32 x;
    s16 y;
    char *s;

    if ((state_word_a & 8) || !(state_word_a & 0x400000) || !D_8015723C) {
        return 1;
    }
    ms = D_801543CC * 1000.0f;
    if (ms < 0) ms = 0;
    text[0] = ms / 600000 % 6 + '0';
    text[1] = ms / 60000 % 10 + '0';
    text[2] = ':';
    text[3] = ms / 10000 % 6 + '0';
    text[4] = ms / 1000 % 10 + '0';
    text[5] = '.';
    text[6] = ms / 100 % 10 + '0';
    text[7] = ms / 10 % 10 + '0';
    text[8] = ms % 10 + '0';
    render_helper(0.0f);
    func_800B669C(1, 3);
    if (D_80151AD0 == 1) {
        font_set(1);
    } else {
        font_set(2);
    }
    w = camera_shake_update('8') + 1;
    old = object_byte9_set(0);
    w2 = camera_shake_update(':') + 1;
    object_byte9_set(old);
    y = D_80115C88[D_80151AD0 - 1].y;
    x = D_80115C88[D_80151AD0 - 1].x - (w * 5 + w2 * 2) / 2;
    ch[1] = 0;
    for (s = text; s != &text[8]; s++) {
        ch[0] = *s;
        if (*s != ':' && *s != '.') {
            ch[0] = '8';
            dispatch_handler(0);
            state_utility(x, y, ch);
            ch[0] = *s;
        }
        dispatch_handler(1);
        state_utility(x, y, ch);
        if (s == &text[1] || s == &text[2] || s == &text[4] || s == &text[5]) {
            x += (w + w2) / 2;
        } else {
            x += w;
        }
    }
    func_800B669C(0, 3);
    render_helper(-1.0f);
    return 1;
}
