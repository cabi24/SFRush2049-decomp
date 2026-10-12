/* flags: -g0 -O3 -mips2 -G 0 -non_shared */
/*
 * func_8010B9C8 (0x8010B9C8, 700 B) -- DRAFT, logic complete (ops 3). Same menu-callback family as
 * func_8010B5D0/func_8010B7FC: disabled when D_801170FC >= 7 (item_disable(item, flag) refreshes
 * the pad config when the flag changes and returns it); draws the header string
 * D_8017A4E0.hdr->unk360 centred, then two option strings strings[info->sel] / [sel + 1] at
 * x -/+ width/4, the selected one (D_80117354) highlighted with dispatch_handler(22).
 * Unit: 108/175 words, 63 rows, ops 3, frame 152/152. Left: the two inlined font_set `old`
 * slots (retail 108/84, ours 100/68) and the named-local block -- the frame layout of the
 * inlined statics is unresolved (unused `s32 i` here is a layout probe, NOT a proven residual);
 * the state_utility argument narrowing temp order (as in the family).
 */
/*
 * func_8010B5D0 (0x8010B5D0, 556 B) -- DRAFT, 7 words. Menu-page callback (same family as
 * func_8010B7FC): one text line centred on D_80117280 -- "%d"-style fcvt of the selected item
 * (D_80117350 set), the language string for modes 7/8, empty for 6, else a formatted player name
 * line (D_8014A118[D_8015698C].name + 20) with its colour D_801146AC[..].
 * Shaping (disclosed): item_refresh() returns the dirty flag (retail keeps it in v1);
 * font_set -> nested static font_select(slot_state_setup) puts font_set's `old` at 68(sp)
 * (each inlined site costs 8 B below it); gfx_lock/gfx_unlock are defined AFTER the function
 * (forward-declared) -- a line-number lever for as1 (sw v0,68 moves out of the jal delay slot);
 * `s32 i` is unused: frame residual (retail 152; 144 without it, with sp offsets 8 B off).
 * Unit: 20/139 words, ops 3, frame 152/152. Left: the s16 narrowing of the first state_utility
 * argument (retail: subu t9; sll a0,t9; sra t6,a0; move a0,t6 -- one word more), and the
 * sw v0,68 / addiu a0 order.
 */
typedef signed char s8;
typedef unsigned char u8;
typedef signed short s16;
typedef unsigned short u16;
typedef signed int s32;
typedef unsigned int u32;
typedef float f32;
typedef struct { u32 opaque[6]; } OSMesgQueue;

typedef struct {
    u8 pad0[0x1A];
    s8 dirty;
} MenuItem;

typedef struct {
    u8 pad0[0x30];
    u16 sel;
    u8 pad32[0x70 - 0x32];
    u16 lang;
} LangInfo;

typedef struct {
    u8 pad0[0x360];
    s32 unk360;
    u8 pad364[0x3A4 - 0x364];
    s32 unk3A4;
} TblHdr;

typedef struct {
    u8 pad0[4];
    TblHdr *hdr;
    u8 pad8[4];
    LangInfo *info;
    s32 *strings;
} StrTable;

extern OSMesgQueue D_801461D0;
extern s16 D_80117280[2];
extern s32 D_80149D98;
extern StrTable D_8017A4E0;

s32 osRecvMesg(OSMesgQueue *, void **, s32);
s32 osSendMesg(OSMesgQueue *, void *, s32);
s32 slot_state_setup(s32 selection);
void Input_ApplyPadConfig(void *);
void render_helper(f32);
s16 object_bytes_sum_global(void);
u32 object_manager_update(void *, s32);
void dispatch_handler(s32);
void fcvt_wrapper(char *, char *, ...);

typedef struct {
    u8 pad0[0x48];
    s32 *name;
} PlayerRec;

extern s8 D_80117350;
extern s8 D_80117354;
extern s32 D_801170FC;
extern s32 D_8015698C;
extern char D_801210CC[];
extern char D_801210E0[];
extern PlayerRec D_8014A118[];
extern s32 D_801146AC[];
void state_utility(s16, s16, void *);

static void gfx_lock(void);
static void gfx_unlock(void);
static s32 font_select(s32 font)
{
    return slot_state_setup(font);
}

static s32 font_set(s32 font)
{
    s32 old;

    gfx_lock();
    old = font_select(font);
    gfx_unlock();
    return old;
}

static s32 item_disable(MenuItem *item, s32 off)
{
    if (off != item->dirty) {
        item->dirty = off;
        Input_ApplyPadConfig(item);
    }
    return item->dirty;
}

s32 func_8010B9C8(MenuItem *item) {
    s32 w;
    s32 x;
    s32 y;
    s32 str;
    s32 i;

    if (item_disable(item, D_801170FC < 7)) {
        return 1;
    }
    render_helper(0.0f);
    font_set(13);
    x = D_80117280[0];
    y = object_bytes_sum_global() * 2 + D_80117280[1];
    font_set(10);
    dispatch_handler(1);
    str = D_8017A4E0.hdr->unk360;
    state_utility((s16)x - (object_manager_update((void *)str, -1) >> 1), y, (void *)str);
    y += object_bytes_sum_global();
    w = object_manager_update((void *)D_8017A4E0.hdr->unk360, -1);
    x = D_80117280[0] - w / 4;
    str = D_8017A4E0.strings[D_8017A4E0.info->sel];
    dispatch_handler(D_80117354 ? 22 : 1);
    state_utility((s16)x - (object_manager_update((void *)str, -1) >> 1), y, (void *)str);
    x = D_80117280[0] + w / 4;
    str = D_8017A4E0.strings[D_8017A4E0.info->sel + 1];
    dispatch_handler(D_80117354 ? 1 : 22);
    state_utility((s16)x - (object_manager_update((void *)str, -1) >> 1), y, (void *)str);
    render_helper(-1.0f);
    return 1;
}

static void gfx_lock(void)
{
    osRecvMesg(&D_801461D0, (void *)0, 1);
}

static void gfx_unlock(void)
{
    osSendMesg(&D_801461D0, (void *)0, 0);
}

