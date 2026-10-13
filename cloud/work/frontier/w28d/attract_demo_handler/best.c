/* flags: -g0 -O3 -mips2 -G 0 -non_shared */
/*
 * attract_demo_handler (0x800DC99C, 820 B) -- WRONG historical label (reached only via control_settings,
 * overlay A). Three-line option cell: cell text centred in the upper box, then three strings with
 * selected/unselected font (10/11) and colour (22/1) by sel1, sel2 and (sel1 || sel2).
 * IPA: row = s5? -- regmap_verify --derive: s6,s7,s5,s8 set in that order (one static site); stack parameters
 * at 64/68/72/76 = o32 positions 2..5. Hypothesis: (row=s6, col=s7, text, s1, s2, s3, sel1=s5, sel2=s8).
 * Lane w28d draft with a stand-in caller (PROVISIONAL at best; needs control_settings). Unit (--internal
 * attract_demo_handler --keep __standin_attract_demo_handler): 198/205 words, rows 35, ops 5, frame 56/56.
 * x reuses the dead `row` parameter (retail s6), the font height has home 44 and y home 48 (y declared first).
 * Residual: parameter colours (ours row/x s5, retail s6 with sel1 in s5; prototype order does not move them)
 * and retail shares the `li a2,1` of the pair store with osRecvMesg's OS_MESG_BLOCK.
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

void control_cell_text3(s32 sel1, s32 row, u8 *text, char *s1, char *s2, char *s3, s32 col, s32 sel2)
{
    Cell *c;
    s32 y;
    s32 fh;

    pair_set(1, 1);
    font_set(11);
    dispatch_handler(1);
    c = &D_80153FD8[row][col];
    camera_auto_follow(c->w / 2 + c->x, (c->h - 40) / 2 + c->y + 20, c->w, c->h - 40, -1, 0, text);
    fh = object_bytes_sum_global();
    row = c->w / 2 + c->x;
    y = c->h + c->y - fh * 3;
    font_set(sel1 ? 10 : 11);
    dispatch_handler(sel1 ? 22 : 1);
    state_utility(row, y - (sel1 ? 1 : 0), s1);
    font_set(sel2 ? 10 : 11);
    dispatch_handler(sel2 ? 22 : 1);
    state_utility(row, y - (sel2 ? 1 : 0) + fh, s2);
    font_set((sel1 || sel2) ? 11 : 10);
    dispatch_handler((sel1 || sel2) ? 1 : 22);
    state_utility(row, y - ((sel1 || sel2) ? 0 : 1) + fh * 2, s3);
    pair_set(0, 3);
}

void __standin_attract_demo_handler(u8 *t, char *a, char *b, char *d, s32 s, s32 u)
{
    s32 i, j;

    for (i = 0; i < 3; i++) {
        for (j = 0; j < 2; j++) {
            control_cell_text3(s, i, t, a, b, d, j, u);
        }
    }
}
