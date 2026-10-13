/* flags: -g0 -O3 -mips2 -G 0 -non_shared */
/*
 * func_8010BC84 (928 B) DRAFT: pause controller-status text. Unit 160/232, ops 29, frame 64/64; EQUIVALENT 90
 * calls (pause_menus). Residual: retail keeps &D_8014A108 (player count, s16) in s8 (address form; `volatile`
 * reloads twice, worse); the `x - (w >> 1)` state_utility argument narrowing (retail subu into a temp, sll a0 /
 * sra tN / move a0); s0/s1 order.
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

static s32 Hidden(Blit *blt, s32 hide)
{
    if (blt->hidden != hide) {
        blt->hidden = hide;
        Input_ApplyPadConfig(blt);
    }
    return blt->hidden;
}

s32 func_8010BC84(Blit *blt)
{
    u16 i;
    s32 ok;
    s16 x;
    s16 y;
    char *str;

    ok = 1;
    if (Hidden(blt, D_801170FC != 6)) {
        return 1;
    }
    render_helper(0.0f);
    for (i = 0; i < D_8014A108; i++) {
        if (!D_80144030[D_8014A118[i].control].b6) {
            ok = 0;
        }
    }
    if (!ok) {
        font_set(10);
        x = D_80117100[0][1].x;
        y = D_80117100[0][1].y - object_bytes_sum_global() / 2;
        dispatch_handler(22);
        str = D_8017A4E0.hdr->title;
        state_utility(x - (object_manager_update(str, -1) >> 1), y, str);
    }
    font_set(11);
    dispatch_handler(1);
    for (i = 0; i < D_8014A108; i++) {
        x = D_80117100[D_8014A108 - 1][i].x;
        y = D_80117100[D_8014A108 - 1][i].y - object_bytes_sum_global();
        dispatch_handler(D_801146AC[i]);
        str = (char *)(*D_8014A118[i].name + 20);
        state_utility(x - (object_manager_update(str, -1) >> 1), y, str);
        y += object_bytes_sum_global();
        if (D_80144030[D_8014A118[i].control].b6) {
            str = D_8017A4E0.hdr->notOk;
        } else {
            str = D_8017A4E0.hdr->ok;
        }
        dispatch_handler(1);
        state_utility(x - (object_manager_update(str, -1) >> 1), y, str);
    }
    render_helper(-1.0f);
    return 1;
}
