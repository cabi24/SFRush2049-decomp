/* flags: -g0 -O3 -mips2 -G 0 -non_shared */
/*
 * func_80108AB0 (0x80108AB0, 760 B) = GC animWrongWay@hud.c ("WRONG WAY" text per window): gRace = D_8014A110,
 * gNumWindows = D_80151AD0, gGameCar = D_80152818 (heading +868, f239 = GC +0xEF), gGameState bit 0x400000,
 * gShowWrongWay = D_801613A0, wrongWayTime = D_801543B8, gSeconds = D_8002EB90, wrongWayPos = D_80115F28,
 * wasStrayed = function-local static (0x8013F2F8, GC wasStrayed$1766), TB_SetJust_8AB0 pair D_80118E20,
 * HAL_SetTextDepth = render_helper, HAL_SetTextTranslucency = func_800ED66C, TB_GetFlashTextCycle_8AB0 = D_8017A630,
 * TB_SetTextColor = dispatch_handler, TB_DrawText = state_utility. GC locals x, y, slot (same here).
 * w28c draft 184/190 -> w30b unit EQUAL. Levers (w30b):
 *  - HAL_SetFont_8AB0 nests one level deeper than the lock/font helper (HAL_SetFont_8AB0 -> font_set_8AB0 -> gfx_lock_8AB0 /
 *    font_select_8AB0 / gfx_unlock_8AB0): with font_select_8AB0 nested the callee-saved cost rises and the FP constants
 *    (+-pi/2, 2.0, 255.0, -1.0) are rematerialised per use as in retail instead of hoisted into $f20-$f26
 *    (184 -> 41 rows); the HAL_SetFont_8AB0 level supplies the last 16 B of frame (two call sites, 8 B each).
 *  - `extern volatile f32 D_8002EB90` (gSeconds): retail reloads it in each arm through an address register (-> 11).
 *  - the reset arm stores wrongWayTime before wasStrayed (web order: wrongWayTime s6, wasStrayed s7 as retail).
 * Shaping devices (disclosed): inlined helpers (non-static *_8AB0, not on keep: w17j stub rule) gfx_lock_8AB0/gfx_unlock_8AB0/font_select_8AB0/font_set_8AB0/HAL_SetFont_8AB0/TB_SetJust_8AB0/
 * TB_GetFlashTextCycle_8AB0 (the GC names; GC calls HAL_SetFont_8AB0/TB_SetJust_8AB0/TB_GetFlashTextCycle_8AB0 as functions, the
 * N64 nesting depth is inferred from the frame); volatile gSeconds.
 */
typedef signed char s8;
typedef unsigned char u8;
typedef signed short s16;
typedef unsigned short u16;
typedef signed int s32;
typedef unsigned int u32;
typedef float f32;
typedef struct { u32 opaque[6]; } OSMesgQueue;

typedef struct TblHdr {
    u8 pad0[0x68];
    char *text;
} TblHdr;

typedef struct StrTable {
    u8 pad0[4];
    TblHdr *hdr;
} StrTable;

typedef struct Pos {
    s32 x, y;
} Pos;

typedef struct Pair {
    s32 h, v;
} Pair;

typedef struct Car {
    u8 pad0[239];
    s8 f239;
    u8 padF0[868 - 240];
    f32 heading;
    u8 pad368[952 - 872];
} Car;

extern OSMesgQueue D_801461D0;
extern u32 D_801174B4;
extern s32 D_8014A110;
extern s8 D_801613A0;
extern Pair D_80118E20;
extern StrTable D_8017A4E0;
extern s16 gNumWindows;
extern Car D_80152818[];
extern f32 D_801543B8[];
extern volatile f32 D_8002EB90;
extern f32 D_8017A630;
extern Pos D_80115F28[][4];

s32 osRecvMesg(OSMesgQueue *, void **, s32);
s32 osSendMesg(OSMesgQueue *, void *, s32);
s32 slot_state_setup(s32 selection);
void render_helper(f32);
void func_800ED66C(f32);
void dispatch_handler(s32);
void state_utility(s16, s16, void *);

void gfx_lock_8AB0(void)
{
    osRecvMesg(&D_801461D0, (void *)0, 1);
}

void gfx_unlock_8AB0(void)
{
    osSendMesg(&D_801461D0, (void *)0, 0);
}

s32 font_select_8AB0(s32 font)
{
    return slot_state_setup(font);
}

s32 font_set_8AB0(s32 font)
{
    s32 old;

    gfx_lock_8AB0();
    old = font_select_8AB0(font);
    gfx_unlock_8AB0();
    return old;
}

void TB_SetJust_8AB0(s32 h, s32 v)
{
    D_80118E20.h = h;
    D_80118E20.v = v;
}

f32 TB_GetFlashTextCycle_8AB0(void)
{
    return D_8017A630;
}

void HAL_SetFont_8AB0(s32 font)
{
    font_set_8AB0(font);
}

s32 animWrongWay(void *item)
{
    s32 x;
    s32 y;
    s32 slot;
    static s8 wasStrayed[4];

    if (D_8014A110 == 5) {
        return 1;
    }
    render_helper(0.0f);
    TB_SetJust_8AB0(1, 1);
    for (slot = 0; slot < gNumWindows; slot++) {
        if ((-1.5707964f <= D_80152818[slot].heading && D_80152818[slot].heading <= 1.5707964f) ||
            !(D_801174B4 & 0x400000) || D_80152818[slot].f239 == 1 || !D_801613A0 || D_8014A110 == 1) {
            D_801543B8[slot] = D_8002EB90;
            wasStrayed[slot] = 0;
            continue;
        }
        wasStrayed[slot] = 1;
        if (D_8002EB90 - D_801543B8[slot] < 2.0f) {
            continue;
        }
        if (gNumWindows == 1) {
            HAL_SetFont_8AB0(10);
        } else {
            HAL_SetFont_8AB0(11);
        }
        func_800ED66C(255.0f * TB_GetFlashTextCycle_8AB0());
        x = D_80115F28[gNumWindows - 1][slot].x;
        y = D_80115F28[gNumWindows - 1][slot].y;
        dispatch_handler(0);
        state_utility(x + 1, y + 1, D_8017A4E0.hdr->text);
        dispatch_handler(14);
        state_utility(x, y, D_8017A4E0.hdr->text);
    }
    TB_SetJust_8AB0(0, 3);
    func_800ED66C(-1.0f);
    render_helper(-1.0f);
    return 1;
}
