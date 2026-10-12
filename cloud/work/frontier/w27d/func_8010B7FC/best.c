/* flags: -g0 -O3 -mips2 -G 0 -non_shared */
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
    y = object_bytes_sum_global() * 2 + D_80117280[1];
    font_set(10);
    for (i = 0; i < 8; i++) {
        dispatch_handler(i == D_80149D98 ? 22 : 1);
        str = D_8017A4E0.strings[D_8017A4E0.info->lang + i + 1];
        state_utility((s16)x - (object_manager_update((void *)str, -1) >> 1), y, (void *)str);
        y += object_bytes_sum_global();
    }
    render_helper(-1.0f);
    return 1;
}
