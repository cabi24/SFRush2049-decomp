/* flags: -g0 -O3 -mips2 -G 0 -non_shared */
/*
 * func_8010BC84 (0x8010BC84, 928 B): pause-screen controller status text (D_801170FC == 6). If any player's
 * controller record (D_80144030[ctl].b6, stride 772) is 0, a centred title (StrTable +0x388, font 10, colour 22) at
 * D_80117100[0][1] - lineheight/2; then per player (count D_8014A108) the player name (record +72 -> +20) and
 * "OK"/"NOT OK" (StrTable +0x2A8/+0x2AC), each centred on D_80117100[n-1][i].x, font 11.
 * MATCH in group / unit EQUAL (w29b; w28c draft 160/232, EQUIVALENT 90 calls on pause_menus).
 * Levers: TB_CenterText(text, centerX) inlined (GC TB_text.c: s16 TB_CenterText(void *text, s16 centerX) =
 * centerX - width/2 narrowed at return; narrowed again for state_utility = TB_DrawText's s16 x: this is the
 * "x - (w >> 1) narrowing through a temp" family residual of B5D0/B7FC/E8B4) 160 -> 99; `ctl` named local + b6 via
 * an inlined getter (value return in v0, as func_80106B3C) in both loops -> EQUAL.
 * Shaping (disclosed): inlined helpers gfx_lock, gfx_unlock, font_set, Hidden, getB6, TB_CenterText (non-static *_BC84, not on keep: w17j).
 */
typedef signed char s8;
typedef unsigned char u8;
typedef signed short s16;
typedef unsigned short u16;
typedef signed int s32;
typedef unsigned int u32;
typedef float f32;
typedef struct { u32 opaque[6]; } OSMesgQueue;

typedef struct Blit {
    u8 pad0[0x1A];
    s8 hidden;
} Blit;

typedef struct TblHdr {
    u8 pad0[0x2A8];
    char *ok;
    char *notOk;
    u8 pad2B0[0x388 - 0x2B0];
    char *title;
} TblHdr;

typedef struct StrTable {
    u8 pad0[4];
    TblHdr *hdr;
} StrTable;

typedef struct Player {
    u8 b0;
    u8 control;
    u8 pad2[70];
    s32 *name;
} Player;

typedef struct Control {
    u8 pad0[6];
    s8 b6;
    u8 pad7[772 - 7];
} Control;

typedef struct TPos {
    u8 pad0[18];
    s16 x;
    s32 y;
} TPos;

extern OSMesgQueue D_801461D0;
extern s32 D_801170FC;
extern s16 D_8014A108;
extern Player D_8014A118[];
extern Control D_80144030[];
extern TPos D_80117100[][4];
extern StrTable D_8017A4E0;
extern s32 D_801146AC[];

s32 osRecvMesg(OSMesgQueue *, void **, s32);
s32 osSendMesg(OSMesgQueue *, void *, s32);
s32 slot_state_setup(s32 selection);
void Input_ApplyPadConfig(void *);
void render_helper(f32);
s16 object_bytes_sum_global(void);
u32 object_manager_update(void *, s32);
void dispatch_handler(s32);
void state_utility(s16, s16, void *);

void gfx_lock_BC84(void)
{
    osRecvMesg(&D_801461D0, (void *)0, 1);
}

void gfx_unlock_BC84(void)
{
    osSendMesg(&D_801461D0, (void *)0, 0);
}

s32 font_set_BC84(s32 font)
{
    s32 old;

    gfx_lock_BC84();
    old = slot_state_setup(font);
    gfx_unlock_BC84();
    return old;
}

s32 Hidden_BC84(Blit *blt, s32 hide)
{
    if (blt->hidden != hide) {
        blt->hidden = hide;
        Input_ApplyPadConfig(blt);
    }
    return blt->hidden;
}

s32 getB6_BC84(s32 ctl)
{
    return D_80144030[ctl].b6;
}

s16 TB_CenterText_BC84(void *text, s16 centerX)
{
    return centerX - (object_manager_update(text, -1) >> 1);
}

s32 func_8010BC84(Blit *blt)
{
    u16 i;
    s32 ok;
    s16 x;
    s16 y;
    char *str;
    s32 ctl;

    ok = 1;
    if (Hidden_BC84(blt, D_801170FC != 6)) {
        return 1;
    }
    render_helper(0.0f);
    for (i = 0; i < D_8014A108; i++) {
        ctl = D_8014A118[i].control;
        if (!getB6_BC84(ctl)) {
            ok = 0;
        }
    }
    if (!ok) {
        font_set_BC84(10);
        x = D_80117100[0][1].x;
        y = D_80117100[0][1].y - object_bytes_sum_global() / 2;
        dispatch_handler(22);
        str = D_8017A4E0.hdr->title;
        state_utility(TB_CenterText_BC84(str, x), y, str);
    }
    font_set_BC84(11);
    dispatch_handler(1);
    for (i = 0; i < D_8014A108; i++) {
        x = D_80117100[D_8014A108 - 1][i].x;
        y = D_80117100[D_8014A108 - 1][i].y - object_bytes_sum_global();
        dispatch_handler(D_801146AC[i]);
        str = (char *)(*D_8014A118[i].name + 20);
        state_utility(TB_CenterText_BC84(str, x), y, str);
        y += object_bytes_sum_global();
        ctl = D_8014A118[i].control;
        if (getB6_BC84(ctl)) {
            str = D_8017A4E0.hdr->notOk;
        } else {
            str = D_8017A4E0.hdr->ok;
        }
        dispatch_handler(1);
        state_utility(TB_CenterText_BC84(str, x), y, str);
    }
    render_helper(-1.0f);
    return 1;
}
