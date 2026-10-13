/* flags: -g0 -O3 -mips2 -G 0 -non_shared */
/*
 * attract_mode_handler (0x800DC88C, 264 B) -- WRONG historical label (w28r census: reached only from overlay
 * image A through control_settings). Draws one menu cell's text centred in its box: font 11 under the
 * D_801461D0 lock (inlined font_set), dispatch_handler(1), pair D_80118E20 = {1, 1}, then
 * camera_auto_follow(x + w/2, y + h/2, w, h, -1, 0, text) for cell D_80153FD8[row][col] (44-byte cells,
 * 2 per row; x/y/w/h s16 at +24/+26/+28/+30), and pair = {0, 3}.
 * IPA register parameters: row = s5, col = s6 (regmap_verify --derive: set at all 7 static sites), text = s7
 * (read without a definition; set once by the caller -- third parameter hypothesis, not proven).
 * Lane w28d: EQUAL in the whole-program unit WITH A STAND-IN CALLER (`--internal attract_mode_handler --keep
 * __standin_attract_mode_handler`) = PROVISIONAL: needs its real caller control_settings (unmatched, 2,552 B).
 * Lever: font_set as ONE inlined level with osRecvMesg/osSendMesg written directly (nested gfx_lock/gfx_unlock
 * helpers move `move s0,v0` into the osSendMesg delay slot: 3 rows).
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

    osRecvMesg(&D_801461D0, (void *)0, 1);
    old = slot_state_setup(font);
    osSendMesg(&D_801461D0, (void *)0, 0);
    return old;
}

static void pair_set(u32 first, u32 second)
{
    Pair *p = &D_80118E20;

    p->first = first;
    p->second = second;
}

void control_cell_text(s32 row, s32 col, u8 *text)
{
    Cell *c;

    font_set(11);
    dispatch_handler(1);
    c = &D_80153FD8[row][col];
    pair_set(1, 1);
    camera_auto_follow(c->w / 2 + c->x, c->h / 2 + c->y, c->w, c->h, -1, 0, text);
    pair_set(0, 3);
}

/* stand-in caller (not a deliverable): gives the IPA unit a caller so the register parameters exist */
void __standin_attract_mode_handler(u8 *t)
{
    s32 i, j;

    for (i = 0; i < 3; i++) {
        for (j = 0; j < 2; j++) {
            control_cell_text(i, j, t);
        }
    }
}
