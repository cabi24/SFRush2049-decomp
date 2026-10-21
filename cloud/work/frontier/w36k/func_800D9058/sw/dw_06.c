/* flags: -g0 -O3 -mips2 -G 0 -non_shared */
/* func_800D9058 = calcControlMenuMaxSize (MAT3 controlscr.c, GC calcControlMenuMaxSize; direct typed port).
 * N64: twelve options (GC thirteen); HAL_SetFont(11) = object_create (locked HAL_SetFont copy, inlined);
 * validOption = func_800D8154 (inlined). Internal in retail: uses s2/s4/s5 without saving (its caller
 * func_800D91A0 relies on that clobber set for its colouring).
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
s32 func_800D8154(s8, s32);              /* validOption */

void func_800D9058(void) {
    s32 rectHeight = D_80113EE0[D_80151AD0 - 1][0].h;
    s32 i;
    s32 gControlMenuCount;
    font_set_CS(11);
    rectHeight -= 24;
    do {} while (0);
    D_8014A10A = rectHeight / 16;
    gControlMenuCount = 2;
    for (i = 0; i < 12; i++) {
        if (func_800D8154(0, i)) gControlMenuCount++;
    }
    if (D_8014A10A > gControlMenuCount || (state_word_a & 0x7C03FFFE))
        D_8014A10A = gControlMenuCount;
}
