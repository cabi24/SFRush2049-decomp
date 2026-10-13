/* flags: -g0 -O3 -mips2 -G 0 -non_shared */
/*
 * func_8010B7FC (0x8010B7FC, 460 B): menu page listing 8 language strings (StrTable strings[lang + i + 1]) centred
 * on D_80117280[0], starting at D_80117280[1] + 2 line heights, selection D_80149D98 in colour 22; font 13 then 10.
 * Levers (w29b): TB_CenterText inlined (GC TB_text.c; the family's narrowing-through-temp residual) 42 -> 2 words;
 * `y = D_80117280[1] + object_bytes_sum_global() * 2` operand order (ugen ring t6 = the shift) -> EQUAL.
 * Body otherwise w27d (item_refresh returns the dirty flag, kept in v1 by retail).
 * MATCH in group / unit EQUAL (w29b). Shaping (disclosed): inlined helpers gfx_lock, gfx_unlock, font_set, item_refresh, TB_CenterText;
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
    u8 pad0[0x70];
    u16 lang;
} LangInfo;

typedef struct {
    u8 pad0[0xC];
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
void state_utility(s16, s16, void *);

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

static s32 item_refresh(MenuItem *item)
{
    if (item->dirty) {
        item->dirty = 0;
        Input_ApplyPadConfig(item);
    }
    return item->dirty;
}

static s16 TB_CenterText(void *text, s16 centerX)
{
    return centerX - (object_manager_update(text, -1) >> 1);
}

s32 func_8010B7FC(MenuItem *item) {
    s32 i;
    s32 x;
    s32 y;
    s32 str;

    if (item_refresh(item)) {
        return 1;
    }
    render_helper(0.0f);
    font_set(13);
    x = D_80117280[0];
    y = D_80117280[1] + object_bytes_sum_global() * 2;
    font_set(10);
    for (i = 0; i < 8; i++) {
        dispatch_handler(i == D_80149D98 ? 22 : 1);
        str = D_8017A4E0.strings[D_8017A4E0.info->lang + i + 1];
        state_utility(TB_CenterText((void *)str, x), y, (void *)str);
        y += object_bytes_sum_global();
    }
    render_helper(-1.0f);
    return 1;
}
