/* flags: -g0 -O3 -mips2 -G 0 -non_shared */
/* func_800D9058 = calcControlMenuMaxSize (controlscr.c; w36n map: proven). Direct typed port of MAT3
 * controlscr.c calcControlMenuMaxSize (GC 0x8006CD08; DWARF locals rectHeight, i, gControlMenuCount).
 * N64: twelve options (GC thirteen); validOption = func_800D8154 (inlined; the unrolled loop compares i with
 * 8/7/6/5); HAL_SetFont(11) = lock / slot_state_setup / unlock, written as non-static helpers (gfx_lock_CS,
 * gfx_unlock_CS, font_set_CS: inlined, internal, deleted stubs carry symbols, w17j) defined AFTER this
 * function (defined before it, as1 schedules `lw s4` into the osRecvMesg delay slot in the group compile).
 * Internal in retail and here: it uses s2/s4/s5 without saving; func_800D91A0's colouring depends on that
 * clobber set (without this body in the unit D91A0 is 51+ words off).
 * `rectHeight -= 24;` gives retail's in-place s4 update.
 * Shaping (disclosed, both compiled out, no code): `if (D_80151AD0) {}` in the loop keeps the &gNumWindows
 * address web live through it (retail's loop constants start at a3), and `do {} while (0);` after the loop
 * (block count: i takes a0 and the count a1). Found by a 86-variant holder sweep (w36k sw/, sw2/); the
 * force.sh oracle had shown the residual was colour-only. No unused locals.
 */
typedef signed short s16;
typedef signed char s8;
typedef signed int s32;
typedef unsigned int u32;
typedef struct { s32 x, y, w, h, cx, cy; } PlayerRect;        /* 0x18 */
extern PlayerRect D_80113EE0[4][4];      /* gPlayerRects */
extern s16 D_80151AD0;                   /* gNumWindows */
extern s16 D_8014A10A;                   /* gControlMenuSize */
extern u32 state_word_a;                 /* gGameState */
typedef struct { u32 opaque[6]; } OSMesgQueue;
extern OSMesgQueue D_801461D0;           /* text/gfx mutex */
s32 osRecvMesg(OSMesgQueue *, void **, s32);
s32 osSendMesg(OSMesgQueue *, void *, s32);
s32 slot_state_setup(s32 font);          /* font select core (IPA s2 parameter) */

void gfx_lock_CS(void);
void gfx_unlock_CS(void);
s32 font_set_CS(s32 font);
s32 func_800D8154(s8, s32);              /* validOption */

void func_800D9058(void) {
    s32 rectHeight = D_80113EE0[D_80151AD0 - 1][0].h;
    s32 i;
    s32 gControlMenuCount;
    font_set_CS(11);
    rectHeight -= 24;
    D_8014A10A = rectHeight / 16;
    gControlMenuCount = 2;
    for (i = 0; i < 12; i++) {
        if (D_80151AD0) {}
        if (func_800D8154(0, i)) gControlMenuCount++;
    }
    do {} while (0);
    if (D_8014A10A > gControlMenuCount || (state_word_a & 0x7C03FFFE))
        D_8014A10A = gControlMenuCount;
}

void gfx_lock_CS(void) { osRecvMesg(&D_801461D0, (void *)0, 1); }
void gfx_unlock_CS(void) { osSendMesg(&D_801461D0, (void *)0, 0); }
s32 font_set_CS(s32 font)                /* HAL_SetFont */
{
    s32 old;

    gfx_lock_CS();
    old = slot_state_setup(font);
    gfx_unlock_CS();
    return old;
}
