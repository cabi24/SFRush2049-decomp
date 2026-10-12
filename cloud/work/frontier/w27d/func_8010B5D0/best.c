/* flags: -g0 -O3 -mips2 -G 0 -non_shared */
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
    u8 pad0[0x70];
    u16 lang;
} LangInfo;

typedef struct {
    u8 pad0[0x3A4];
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

static s32 item_refresh(MenuItem *item)
{
    if (item->dirty) {
        item->dirty = 0;
        Input_ApplyPadConfig(item);
    }
    return item->dirty;
}

s32 func_8010B5D0(MenuItem *item) {
    s32 i;
    s32 x;
    s32 y;
    s32 str;
    char buf[40];
    s32 v;

    if (item_refresh(item)) {
        return 1;
    }
    render_helper(0.0f);
    font_set(13);
    x = D_80117280[0];
    y = D_80117280[1];
    str = (s32)buf;
    if (D_80117350) {
        fcvt_wrapper(buf, D_801210CC, D_8015698C + 1, D_8017A4E0.hdr->unk3A4);
        dispatch_handler(1);
    } else {
        v = D_801170FC;
        if (v == 7 || v == 8) {
            str = D_8017A4E0.strings[D_8017A4E0.info->lang + v];
            dispatch_handler(1);
        } else if (v == 6) {
            buf[0] = 0;
        } else {
            fcvt_wrapper(buf, D_801210E0, *D_8014A118[D_8015698C].name + 20, D_8017A4E0.strings[D_8017A4E0.info->lang]);
            dispatch_handler(D_801146AC[D_8015698C]);
        }
    }
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

