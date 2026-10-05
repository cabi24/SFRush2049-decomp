/* flags: -g0 -O3 -mips2 -G 0 -non_shared  (also code-identical at -O2) */
/*
 * func_80106D94: Blit AnimFunc of the HUD odometer digits (N64-only; no arcade
 * ancestor found, the arcade Blit layout is shifted: X 0x0E, Y 0x10, Width
 * 0x14, Hide 0x1A, Top/Bot/Left/Right 0x1C..0x22, AnimDTA 0x28, AnimID 0x2C).
 * AnimID bits 4-7 = player slot, bits 0-3 = digit (0 hundreds .. 3 tenths).
 * The distance is car[slot]+0x108 in feet (divided by 0.6f when D_80146111 is
 * set), 528 ft = 0.1 mile.  The blit is hidden when the slot is not on screen
 * (slot >= D_80151AD0, or D_8014A110 == 5), when D_8015F734 is 0, or when the
 * followed car's byte 0xEF is 1.  Each digit is a window of height Width/2
 * into a vertical strip of numerals; a digit scrolls by the tenths fraction
 * only while every lower digit shows 9.  Screen position comes from
 * D_80115AE8[D_80151AD0 - 1][slot] ({x, y} pairs; D_80151AD0 looks like the
 * number of views, INFERRED, as is "feet" from the 5280 constants).
 *
 * STATE: code identical (401/401 words), own-rodata unverified by the
 * unpatched scorer.  Literals checked by hand against the image:
 *   0x801248B4 3F19999A 0.6f, 0x801248B8 4900E800 528000.0f,
 *   0x801248BC 474E4000 52800.0f, 0x801248C0 474E4000 52800.0f
 * (5280.0f and 528.0f are `lui` immediates).
 *
 * Shaping (what closed it):
 *   - there is no half-width local: every use is `blt->Width / 2` (macro HW)
 *     and uopt's PRE supplies the lone `Width / 2` on the default path.  With a
 *     local `w` assigned at the top of each case, case 3 is 1 word longer
 *     (`sll v0,t1,3` stays in the block instead of the `beq` delay slot)
 *     because ugen then emits the slot*8 CSE after the divide.
 *   - `itenths % 10 == 9` is repeated, not held in a local (operand order of
 *     the `bnel`).
 *   - `hide = blt->Hide; if (hide)` supplies the `move v0,v1`.
 *   - frame: declaration order slot, digit, <5 int locals>, dist, frac,
 *     tenths gives the 64-byte frame and the spill slots 60/56/32/28; three
 *     of the five are unused (unused0..2), so the original list is unknown.
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
    /* 0x18 */ u8 pad18[2];
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
    /* 0x0F0 */ char padF0[0x18];
    /* 0x108 */ f32 distance;
    /* 0x10C */ char pad10C[0x2AC];
} Car; /* 0x3B8 */

typedef struct {
    /* 0x000 */ char pad0[0x7C6];
    /* 0x7C6 */ s16 unk7C6;
    /* 0x7C8 */ char pad7C8[0x40];
} Model; /* 0x808 */

typedef struct {
    s32 x;
    s32 y;
} Pos;

extern Car D_80152818[];
extern Model D_8014A250[];
extern s8 D_80146111;
extern s16 D_80151AD0;
extern s32 D_8014A110;
extern s8 D_8015F734;
extern Pos D_80115AE8[][4];

void Input_ApplyPadConfig(Blit *blt);

#define HW (blt->Width / 2)

s32 func_80106D94(Blit *blt) {
    s32 slot;
    s32 digit;
    s32 unused0;
    s32 itenths;
    s32 hide;
    s32 unused1;
    s32 unused2;
    f32 dist;
    f32 frac;
    f32 tenths;

    slot = (blt->AnimID & 0xF0) >> 4;
    digit = blt->AnimID & 0xF;
    frac = 0.0f;
    dist = D_80152818[slot].distance;
    if (D_80146111) {
        dist = dist / 0.6f;
    }
    if (!(slot < D_80151AD0) || D_8014A110 == 5) {
        blt->AnimDTA = 0;
        if (blt->Hide != 1) {
            blt->Hide = 1;
            Input_ApplyPadConfig(blt);
        }
        return 1;
    }
    hide = D_8015F734 == 0 || D_80152818[D_8014A250[slot].unk7C6].unkEF == 1;
    if (hide != blt->Hide) {
        blt->Hide = hide;
        Input_ApplyPadConfig(blt);
    }
    hide = blt->Hide;
    if (hide) {
        return 1;
    }
    tenths = dist / 528.0f;
    itenths = tenths;
    if (itenths % 10 == 9) {
        frac = tenths - itenths;
    }
    switch (digit) {
    case 0:
        blt->X = D_80115AE8[D_80151AD0 - 1][slot].x - HW * 2;
        blt->Y = D_80115AE8[D_80151AD0 - 1][slot].y;
        blt->Left = 0;
        blt->Top = ((s32) (dist / 528000.0f) % 10) * HW;
        if ((s32) (dist / 52800.0f) % 10 == 9 && (s32) (dist / 5280.0f) % 10 == 9 && itenths % 10 == 9) {
            blt->Top = blt->Top + frac * HW;
        }
        break;
    case 1:
        blt->X = D_80115AE8[D_80151AD0 - 1][slot].x - HW;
        blt->Y = D_80115AE8[D_80151AD0 - 1][slot].y;
        blt->Left = 0;
        blt->Top = ((s32) (dist / 52800.0f) % 10) * HW;
        if ((s32) (dist / 5280.0f) % 10 == 9 && itenths % 10 == 9) {
            blt->Top = blt->Top + frac * HW;
        }
        break;
    case 2:
        blt->X = D_80115AE8[D_80151AD0 - 1][slot].x;
        blt->Y = D_80115AE8[D_80151AD0 - 1][slot].y;
        blt->Left = 0;
        blt->Top = ((s32) (dist / 5280.0f) % 10) * HW;
        if (itenths % 10 == 9) {
            blt->Top = blt->Top + frac * HW;
        }
        break;
    case 3:
        blt->X = D_80115AE8[D_80151AD0 - 1][slot].x + HW;
        blt->Y = D_80115AE8[D_80151AD0 - 1][slot].y;
        blt->Left = HW;
        blt->Top = HW * (itenths % 10 + tenths - itenths);
        break;
    }
    blt->Right = blt->Left + HW - 1;
    blt->Bot = blt->Top + HW - 1;
    Input_ApplyPadConfig(blt);
    return 1;
}
