/* flags: -g0 -O3 -mips2 -G 0 -non_shared */
/* Image A, 0x803A4134..0x803A4340: select-screen stat-bar BLIT callback.
 * Arcade ancestry: historicalsource/rushtherock 845329d7b36f5a384c5625ed9a0aef584ab46139,
 * game/select.c:2383 AnimateBar. This is N64-specific layout/control flow,
 * not a verbatim donor: packed player/bar/segment, once-only initialization,
 * signed geometry, visibility and the selected float-product table differ.
 * sound_control/NewMultiBlit proves +40 AnimFunc and +44 AnimID; the record
 * below is only the accessed native prefix, not a full-size BLIT assertion.
 * Initialized callbacks require player/bar 0..3 before their table accesses.
 * The native float path rounds multiply/subtract to binary32, truncates to
 * signed32, then stores a halfword before applying the signed minimum of 2.
 * C float-to-s16 behavior is claimed only for finite, representable truncated
 * values. Native wider narrowing is separately tested, not called portable C.
 * The initialized local is the genuinely consumed high-bit state. No dummy
 * reads, padding locals, stand-in callees, or compiler-shaping work is used.
 */
typedef unsigned char u8;
typedef signed char s8;
typedef signed short s16;
typedef unsigned short u16;
typedef signed int s32;
typedef unsigned int u32;
typedef struct Blit {
    char *Name;
    void *Image;
    void *Info;
    s16 TexIndex, X, Y;
    u16 State;
    s16 Width, Height;
    u8 Alpha, Flip;
    s8 Hide;
    u8 Init;
    s16 Top, Bot, Left, Right, Color;
    u16 reserved26;
    s32 (*AnimFunc)(struct Blit *);
    u32 AnimID;
} Blit;
extern s16 D_8014A108;
extern s32 D_8014A110;
extern char D_803B85D8[];
extern s8 D_803BA028[];
extern float D_803BA190[][4];
extern void func_800EF5B0(Blit *, char *, s32);
extern s8 input_new_data_wrapper(Blit *, s32);
extern void Input_ApplyPadConfig(Blit *);
s32 func_803A4134(Blit *blt)
{
    s32 segment = blt->AnimID & 15;
    s32 bar = (blt->AnimID >> 4) & 15;
    s32 player = (blt->AnimID >> 8) & 15;
    s32 height;
    s32 initialized = blt->AnimID >> 31;
    if (!initialized) {
        if (D_8014A108 >= 3 && player < 2)
            func_800EF5B0(blt, D_803B85D8, 0);
        blt->AnimID |= 0x80000000;
        if (player < D_8014A108 && D_8014A108 == 1 && D_8014A110 != 2) {
            blt->X = 10;
            height = blt->Height / 8;
            blt->Y = 48 + bar * 2 * height;
        } else {
            input_new_data_wrapper(blt, 1);
            blt->AnimFunc = 0;
            return 1;
        }
        blt->Left = 0;
        blt->Top = (bar * 2 + (segment == 0)) * height;
        blt->Bot = blt->Top + height - 1;
    }
    if (input_new_data_wrapper(blt, D_803BA028[player] == 1))
        return 1;
    if (segment == 1) {
        blt->Right = D_803BA190[player][bar] * blt->Width - 1;
        blt->Right = blt->Right < 2 ? 2 : blt->Right;
    } else
        blt->Right = blt->Width - 1;
    Input_ApplyPadConfig(blt);
    return 1;
}
