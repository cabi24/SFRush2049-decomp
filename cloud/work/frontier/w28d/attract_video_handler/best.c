/* flags: -g0 -O3 -mips2 -G 0 -non_shared */
/*
 * attract_video_handler (0x800DCDF4, 692 B) -- WRONG historical label (reached only via control_settings,
 * overlay A). Draws a two-line option cell: the cell text centred in the upper box (camera_auto_follow with the
 * box height - 40, +20), then two strings (D_8017A4E0[1]->+944 and the D_8017A4E0[4] table entry indexed by
 * D_8017A4E0[3]->+50) with the selected/unselected font (11/10) and colour (1/22) swapped by `sel`.
 * IPA register parameters (hypothesis from the body): sel = s5, row = s6, col = s7, text = s8.
 * Lane w28d draft with a stand-in caller (PROVISIONAL at best; needs control_settings). Unit (--internal
 * attract_video_handler --keep __standin_attract_video_handler): 18/173 words, rows 17, ops 2, frame 48/48.
 * Levers: x and the font height reuse the dead parameters text (s8) and col (s7) -- retail has only two named
 * homes (c, y); three nested font_set sites. Residual: retail emits the first inlined font_set return copy
 * `move v0,s0` and builds the cell pointer straight into s0 (ours v0 then move). text typed s32 here.
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
typedef struct {
    u8 pad00[24];
    s16 x, y, w, h;     /* 24..30 */
    u8 pad20[12];
} Cell;                 /* 44 bytes */
extern OSMesgQueue D_801461D0;
extern Pair D_80118E20;
extern Cell D_80153FD8[][2];
extern s32 osRecvMesg(OSMesgQueue *, void **, s32);
extern s32 osSendMesg(OSMesgQueue *, void *, s32);
extern s32 slot_state_setup(s32 selection);
extern void dispatch_handler(s32);
extern u8 *D_8017A4E0[];
extern s32 object_bytes_sum_global(void);
extern void state_utility(s16, s16, char *);
extern void camera_auto_follow(s16 arg0, s16 arg1, s16 arg2, s16 arg3, s16 arg4, s16 arg5, u8 *arg6);

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

static void pair_set(u32 first, u32 second)
{
    Pair *p = &D_80118E20;

    p->first = first;
    p->second = second;
}

void attract_video_handler(s32 sel, s32 row, s32 col, s32 text)
{
    Cell *c;
    s32 y;

    dispatch_handler(1);
    font_set(11);
    c = &D_80153FD8[row][col];
    pair_set(1, 1);
    camera_auto_follow(c->w / 2 + c->x, (c->h - 40) / 2 + c->y + 20, c->w, c->h - 40, -1, 0, (u8 *)text);
    col = object_bytes_sum_global();
    text = c->w / 2 + c->x;
    y = c->h + c->y - col * 3;
    font_set(sel ? 11 : 10);
    dispatch_handler(sel ? 1 : 22);
    state_utility(text, y - (sel ? 0 : 1) + col, *(char **)(D_8017A4E0[1] + 944));
    font_set(sel ? 10 : 11);
    dispatch_handler(sel ? 22 : 1);
    state_utility(text, y - (sel ? 1 : 0), ((char **)D_8017A4E0[4])[*(u16 *)(D_8017A4E0[3] + 50)]);
    pair_set(0, 3);
}

void __standin_attract_video_handler(s32 s, s32 t)
{
    s32 i, j;

    for (i = 0; i < 3; i++) {
        for (j = 0; j < 2; j++) {
            attract_video_handler(s, i, j, t);
        }
    }
}
