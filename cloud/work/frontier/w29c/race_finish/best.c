/* flags: -g0 -O3 -mips2 -G 0 -non_shared */
/*
 * race_finish (0x80107AF4, 1,000 B), historical label; = GC hud.c animPlace (static int animPlace(void); GC locals
 * digits[3], str[2], digitW, x, y, digitIdx, slot). Per-player race position "P/N" HUD (place + 1, '/', car
 * count), drawn glyph by glyph with a shadow pass at (x+1, y+1), place digit in the player colour D_80116168[i].
 * Lanes w28d/w29c. NOT a match. Unit (us.sh): 40/250 words, rows 40, ops 6, frame 280/280 (w28d: rows 100).
 * Levers: font_set (lock + slot_state_setup[s2] + unlock) inlined at 6 sites; D_80118E20 (TB_SetJust pair) set by
 * an inlined pair_set(s32, s32) with a `Pair *p` local storing .second before .first (the matched sibling
 * func_80107EDC writes `.b = 3; .a = 1;` on an s32 pair): the s32 1 joins retail's v0 constant piece and the
 * two inlined sites size the frame (280); state_utility(s16, s16, char *) (retail narrows both at every call);
 * y is an s32 whose (s16) conversions are spill temps; the glyph loop is index form (k).
 * DEVICE (disclosed): compiled-out `if (i) {}` after the glyph loop (end of the outer loop body) raises i's
 * priority above the int-constant-1 web so i gets s8 as in retail (without it: rows 88, the 1-web takes s8).
 * Residual (colour/schedule): player_array[i] address in v1 (retail a0) and y in a0 (retail v1); the outer
 * block loads x before y (retail y, place, x, w); spill homes of (s16)(y+1)/(s16)y at 72/68 (retail 76/72).
 */
typedef signed char s8;
typedef unsigned char u8;
typedef signed short s16;
typedef unsigned short u16;
typedef signed int s32;
typedef unsigned int u32;
typedef float f32;
typedef struct { u32 opaque[6]; } OSMesgQueue;
typedef struct { s32 first, second; } Pair;
typedef struct { s32 x, y; } Pos;
typedef struct { u8 r, g, b, a; } Rgba;
typedef struct {
    u8 pad000[238];
    s8 place;        /* 238 */
    s8 finished;     /* 239 */
    u8 pad0F0[952 - 240];
} Player952;
extern OSMesgQueue D_801461D0;
extern s32 state_word_a;
extern s16 D_801543CA;
extern s32 gameplay_mode;
extern s8 D_8016137C;
extern s16 gNumWindows;
extern Pair D_80118E20;
extern Pos D_80115CA8[4][4];
extern Rgba D_80116168[];
extern Player952 player_array[];
extern s32 osRecvMesg(OSMesgQueue *, void **, s32);
extern s32 osSendMesg(OSMesgQueue *, void *, s32);
extern s32 slot_state_setup(s32 selection);
extern void render_helper(f32);
extern s32 camera_shake_update(u16 id);
extern void dispatch_handler(s32);
extern void state_utility(s16, s16, char *);
extern s32 func_800CF604(s16);
extern void func_800B7360(s32, s32, s32, s32);

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

static void pair_set(s32 first, s32 second)
{
    Pair *p = &D_80118E20;

    p->second = second;
    p->first = first;
}

s32 animPlace(s32 arg0)
{
    char str[4];
    char ch[2];
    s32 w;
    s32 i;
    s32 x;
    s32 y;
    s32 k;

    if ((state_word_a & 8) || D_801543CA < 2 || gameplay_mode == 1 || !D_8016137C) {
        return 1;
    }
    render_helper(0.0f);
    pair_set(1, 3);
    if (gNumWindows == 1) {
        font_set(8);
    } else {
        font_set(5);
    }
    w = camera_shake_update('8');
    str[1] = '/';
    str[2] = D_801543CA + '0';
    for (i = 0; i < gNumWindows; i++) {
        if (func_800CF604(i) == 0 || player_array[i].finished == 1) {
            continue;
        }
        y = D_80115CA8[gNumWindows - 1][i].y;
        x = D_80115CA8[gNumWindows - 1][i].x - w;
        str[0] = player_array[i].place + '1';
        for (k = 0; k < 3; k++) {
            ch[0] = str[k];
            ch[1] = 0;
            if (k < 2) {
                if (gNumWindows == 1) {
                    font_set(8);
                } else {
                    font_set(5);
                }
            } else {
                if (gNumWindows == 1) {
                    font_set(5);
                } else {
                    font_set(6);
                }
            }
            dispatch_handler(0);
            state_utility(x + 1, y + 1, ch);
            if (k == 0) {
                func_800B7360(D_80116168[i].r, D_80116168[i].g, D_80116168[i].b, 255);
            } else {
                dispatch_handler(1);
            }
            state_utility(x, y, ch);
            x += w;
        }
        if (i) {}
    }
    pair_set(0, 3);
    render_helper(-1.0f);
    return 1;
}
