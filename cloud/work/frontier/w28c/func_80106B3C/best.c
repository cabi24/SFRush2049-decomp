/* flags: -g0 -O3 -mips2 -G 0 -non_shared */
/*
 * func_80106B3C (600 B) DRAFT: unit 50/150 words, rows 52, ops 16, frame 64/64; draft_check EQUIVALENT 4,832
 * calls (race, pause_menus). HUD title text per window (hidden unless D_801174B4 bit 22). Inlined font_set
 * (first switch; its returned old font is the retail `move s0,v0`), Hidden helper. Residual: colour (nw kept in
 * a1 by retail; loop invariants s5-s7 order).
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
    u8 pad1B[0x28 - 0x1B];
    s32 state;
} Blit;

typedef struct TblHdr {
    u8 pad0[0x388];
    char *text;
} TblHdr;

typedef struct Player {
    u8 b0;
    u8 control;
    u8 pad2[74];
} Player;

typedef struct Control {
    u8 pad0[6];
    s8 b6;
    u8 pad7[772 - 7];
} Control;

typedef struct Pos {
    s32 x, y;
} Pos;

typedef struct Pair {
    s32 a, b;
} Pair;

extern OSMesgQueue D_801461D0;
extern u32 D_801174B4;
extern Pair D_80118E20;
extern TblHdr *D_8017A4E4;
extern s16 gNumWindows;
extern s32 D_801170FC;
extern Player D_8014A118[];
extern Control D_80144030[];
extern Pos D_80115FA8[][4];
extern s32 D_801146AC[];

s32 osRecvMesg(OSMesgQueue *, void **, s32);
s32 osSendMesg(OSMesgQueue *, void *, s32);
s32 slot_state_setup(s32 selection);
void Input_ApplyPadConfig(void *);
u32 object_manager_update(void *, s32);
void dispatch_handler(s32);
void state_utility(s16, s16, void *);

static s32 Hidden(Blit *blt, s32 hide)
{
    if (blt->hidden != hide) {
        blt->hidden = hide;
        Input_ApplyPadConfig(blt);
    }
    return blt->hidden;
}

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

s32 func_80106B3C(Blit *blt)
{
    u32 width;
    s32 limit;
    s32 i;
    s32 x;
    s32 y;

    if (Hidden(blt, D_801174B4 & 0x400000)) {
        blt->state = 0;
        return 0;
    }
    D_80118E20.a = 1;
    D_80118E20.b = 1;
    font_set(11);
    width = object_manager_update(D_8017A4E4->text, -1);
    limit = (gNumWindows >= 3) ? 160 : ((gNumWindows >= 2) ? 240 : 320);
    if (limit + 8 < width) {
        gfx_lock();
        slot_state_setup(12);
        gfx_unlock();
    }
    for (i = 0; i < gNumWindows; i++) {
        if ((D_801174B4 & 0x200000) && !D_801170FC) {
            if (!D_80144030[D_8014A118[i].control].b6) {
                x = D_80115FA8[gNumWindows - 1][i].x;
                y = D_80115FA8[gNumWindows - 1][i].y;
                dispatch_handler(D_801146AC[i]);
                state_utility(x, y, D_8017A4E4->text);
            }
        }
    }
    D_80118E20.b = 3;
    D_80118E20.a = 0;
    return 1;
}
