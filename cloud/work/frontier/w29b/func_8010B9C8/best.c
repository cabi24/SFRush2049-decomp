/* flags: -g0 -O3 -mips2 -G 0 -non_shared */
/*
 * func_8010B9C8 (0x8010B9C8, 700 B): two-choice menu page (header string centred, two option strings at x -/+ w/4,
 * selection D_80117354 highlighted), disabled while D_801170FC >= 7 (item_disable returns the flag).
 * Levers (w29b): TB_CenterText inlined (GC TB_text.c) 108 -> 30 words; dropping w27d's frame devices (nested
 * font_select, unused `s32 i`: the inlined TB_CenterText sites now give retail's 152-byte frame) -> 3; `y =
 * D_80117280[1] + object_bytes_sum_global() * 2` operand order -> EQUAL. No unused locals.
 * MATCH in group / unit EQUAL (w29b). Shaping (disclosed): inlined helpers gfx_lock, gfx_unlock, font_set, item_disable, TB_CenterText;
 * gfx_lock/gfx_unlock definitions placement as in w27d (as1 line tie).
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
static s32 font_set(s32 font)
{
    s32 old;

    gfx_lock();
    old = slot_state_setup(font);
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

static s16 TB_CenterText(void *text, s16 centerX)
{
    return centerX - (object_manager_update(text, -1) >> 1);
}

s32 func_8010B9C8(MenuItem *item) {
    s32 w;
    s32 x;
    s32 y;
    s32 str;

    if (item_disable(item, D_801170FC < 7)) {
        return 1;
    }
    render_helper(0.0f);
    font_set(13);
    x = D_80117280[0];
    y = D_80117280[1] + object_bytes_sum_global() * 2;
    font_set(10);
    dispatch_handler(1);
    str = D_8017A4E0.hdr->unk360;
    state_utility(TB_CenterText((void *)str, x), y, (void *)str);
    y += object_bytes_sum_global();
    w = object_manager_update((void *)D_8017A4E0.hdr->unk360, -1);
    x = D_80117280[0] - w / 4;
    str = D_8017A4E0.strings[D_8017A4E0.info->sel];
    dispatch_handler(D_80117354 ? 22 : 1);
    state_utility(TB_CenterText((void *)str, x), y, (void *)str);
    x = D_80117280[0] + w / 4;
    str = D_8017A4E0.strings[D_8017A4E0.info->sel + 1];
    dispatch_handler(D_80117354 ? 1 : 22);
    state_utility(TB_CenterText((void *)str, x), y, (void *)str);
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

