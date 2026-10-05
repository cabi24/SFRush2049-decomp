/* flags: -g0 -O3 -mips2 -G 0 -non_shared */
/*
 * func_800EF62C: Blit AnimFunc of the split-screen HUD tachometer (N64-only; the arcade
 * AnimateTach/AnimateNeedle in game/hud.c draw a needle object instead). Sibling of the
 * locked odometer AnimFunc func_80106D94 (same Blit layout and view-slot conventions).
 * AnimID bits 4-7 = view slot, bits 0-3 = part (0 frame, 1 bar).
 *  - slot >= D_80151AD0 (views on screen): AnimDTA is cleared when the slot is also past
 *    D_8014A108, the blit is hidden (UpdateBlit = Input_ApplyPadConfig on a change).
 *  - otherwise hidden when D_8015B25C is 0, the view's model byte +0x0A is set, or the
 *    followed car's byte +0xEF is 1 (same test as func_80106D94).
 *  - 2 views: RenameBlit(func_800EF5B0) to "TACHOMETER_MD", 3-4 views: "TACHOMETER_SM";
 *    position from D_80115B68[views - 1][slot] ({x, y} pairs), centred on Width.
 *  - part 0: Bot = Height/2 - 1. Part 1: the bar is clipped to Right = (Width - 1) * f with
 *    f = |rpm * 1.35 * 0.0001| clamped to 1 (arcade AnimateNeedle: rpm * 1.35 is the "fake
 *    rpm boost"), Top = Height/2, Bot = Height - 1.  Alpha 254, then UpdateBlit.
 *
 * STATE: code identical (178/178 words); own literals unverifiable by the scorer because the
 * object's .rodata holds both the two strings (retail: .data-area 0x80120D9C/0x80120DAC) and
 * the two floats (retail .rodata 0x801245A4/0x801245A8). Bytes checked by hand against
 * build/game_code.bin: "TACHOMETER_MD\0\0\0", "TACHOMETER_SM\0\0\0", 3FACCCCD (1.35f),
 * 38D1B717 (0.0001f). EQUAL in the whole-program unit (blob_unit score, kept).
 *
 * Shaping: the Right/Top/Bot stores must be written Right, Top, Bot (the other five orders
 * give a t8/t9/t0 temp permutation or worse).
 */
typedef signed char s8;
typedef unsigned char u8;
typedef signed short s16;
typedef unsigned short u16;
typedef signed int s32;
typedef unsigned int u32;
typedef float f32;

typedef struct Blit {
    /* 0x00 */ char pad0[0xE];
    /* 0x0E */ s16 X;
    /* 0x10 */ s16 Y;
    /* 0x12 */ s16 pad12;
    /* 0x14 */ s16 Width;
    /* 0x16 */ s16 Height;
    /* 0x18 */ u8 Alpha;
    /* 0x19 */ u8 pad19;
    /* 0x1A */ s8 Hide;
    /* 0x1C */ s16 Top;
    /* 0x1E */ s16 Bot;
    /* 0x20 */ s16 Left;
    /* 0x22 */ s16 Right;
    /* 0x24 */ s32 pad24;
    /* 0x28 */ s32 AnimDTA;
    /* 0x2C */ u32 AnimID;
} Blit;

typedef struct {
    /* 0x000 */ char pad0[0xEF];
    /* 0x0EF */ s8 unkEF;
    /* 0x0F0 */ char padF0[0x2C8];
} Car; /* 0x3B8 */

typedef struct {
    /* 0x000 */ char pad0[0xA];
    /* 0x00A */ s8 unkA;
    /* 0x00B */ char padB[0x7BB];
    /* 0x7C6 */ s16 unk7C6;
    /* 0x7C8 */ char pad7C8[8];
    /* 0x7D0 */ s16 rpm;
    /* 0x7D2 */ char pad7D2[0x36];
} Model; /* 0x808 */

typedef struct {
    s32 x;
    s32 y;
} Pos;

extern Car D_80152818[];
extern Model D_8014A250[];
extern s16 D_80151AD0;
extern s16 D_8014A108;
extern s8 D_8015B25C;
extern Pos D_80115B68[][4];

void Input_ApplyPadConfig(Blit *blt);
void func_800EF5B0(Blit *blt, char *name, s32 preserve);

s32 func_800EF62C(Blit *blt) {
    s32 slot;
    s32 type;
    s32 hide;
    f32 temp;

    slot = (blt->AnimID & 0xF0) >> 4;
    type = blt->AnimID & 0xF;
    if (!(slot < D_80151AD0)) {
        if (!(slot < D_8014A108)) {
            blt->AnimDTA = 0;
        }
        if (blt->Hide != 1) {
            blt->Hide = 1;
            Input_ApplyPadConfig(blt);
        }
        return 1;
    }
    hide = D_8015B25C == 0 || D_8014A250[slot].unkA != 0 || D_80152818[D_8014A250[slot].unk7C6].unkEF == 1;
    if (hide != blt->Hide) {
        blt->Hide = hide;
        Input_ApplyPadConfig(blt);
    }
    if (blt->Hide) {
        return 1;
    }
    if (D_80151AD0 == 2) {
        func_800EF5B0(blt, "TACHOMETER_MD", 0);
    } else if (D_80151AD0 >= 3) {
        func_800EF5B0(blt, "TACHOMETER_SM", 0);
    }
    blt->X = D_80115B68[D_80151AD0 - 1][slot].x - blt->Width / 2;
    blt->Y = D_80115B68[D_80151AD0 - 1][slot].y;
    if (type == 0) {
        blt->Bot = blt->Height / 2 - 1;
    } else if (type == 1) {
        temp = D_8014A250[slot].rpm * 1.35f * 0.0001f;
        if (temp < 0.0f) {
            temp = -temp;
        }
        if (temp > 1.0f) {
            temp = 1.0f;
        }
        blt->Right = (blt->Width - 1) * temp;
        blt->Top = blt->Height / 2;
        blt->Bot = blt->Height - 1;
    }
    blt->Alpha = 254;
    Input_ApplyPadConfig(blt);
    return 1;
}
