/* flags: -g0 -O3 -mips2 -G 0 -non_shared */
/*
 * func_80106B3C (0x80106B3C, 600 B): per-window HUD title text (StrTable +0x388), hidden unless D_801174B4 bit 22;
 * font 11, or font 12 when the text is wider than the window limit + 8; drawn for each window whose player's
 * controller record (D_80144030[ctl], stride 772) has b6 == 0, while D_801174B4 bit 21 is set and D_801170FC == 0.
 * MATCH in group / unit EQUAL (w29b; w28c draft was 50/150, EQUIVALENT 4,832 calls on race + pause_menus).
 * Levers: TB_SetJust(1,1)/(0,3) = the locked func_800B669C body inlined (pointer to D_80118E20; GC name from w28c);
 * `ctl = D_8014A118[i].control` as a named local (fixes s5-s7); the b6 test through an inlined getter whose
 * value return takes v0 (w15 inlined-value-return mechanism; a named local `b` got v1); the lock/font helpers
 * DEFINED AFTER the function (prototypes before): as1 breaks the osRecvMesg-argument vs TB_SetJust-store tie by
 * .loc line, retail's stores come first.
 * Shaping (disclosed): inlined helpers Hidden, TB_SetJust, getB6, gfx_lock, gfx_unlock, font_set (identity of
 * getB6/TB_SetJust as separate source helpers is a hypothesis; the font_set family is the established idiom);
 * helper definition placement after the function (as1 line-number tie).
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

static void TB_SetJust(u32 first, u32 second)
{
    Pair *p = &D_80118E20;
    p->a = first;
    p->b = second;
}

static s32 Hidden(Blit *blt, s32 hide)
{
    if (blt->hidden != hide) {
        blt->hidden = hide;
        Input_ApplyPadConfig(blt);
    }
    return blt->hidden;
}

static void gfx_lock(void);
static void gfx_unlock(void);
static s32 font_set(s32 font);

static s32 getB6(s32 ctl)
{
    return D_80144030[ctl].b6;
}

s32 func_80106B3C(Blit *blt)
{
    u32 width;
    s32 limit;
    s32 i;
    s32 x;
    s32 y;
    s32 ctl;

    if (Hidden(blt, D_801174B4 & 0x400000)) {
        blt->state = 0;
        return 0;
    }
    TB_SetJust(1, 1);
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
            ctl = D_8014A118[i].control;
            if (!getB6(ctl)) {
                x = D_80115FA8[gNumWindows - 1][i].x;
                y = D_80115FA8[gNumWindows - 1][i].y;
                dispatch_handler(D_801146AC[i]);
                state_utility(x, y, D_8017A4E4->text);
            }
        }
    }
    TB_SetJust(0, 3);
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

static s32 font_set(s32 font)
{
    s32 old;

    gfx_lock();
    old = slot_state_setup(font);
    gfx_unlock();
    return old;
}

