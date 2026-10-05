/* flags: -g0 -O3 -mips2 -G 0 -non_shared */
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

s32 func_80106D94(Blit *blt) {
    s32 slot;
    s32 digit;
    s32 w;
    s32 itenths;
    s32 hide;
    s32 p1;
    s32 p2;
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
        w = blt->Width / 2;
        blt->X = D_80115AE8[D_80151AD0 - 1][slot].x - w * 2;
        blt->Y = D_80115AE8[D_80151AD0 - 1][slot].y;
        blt->Left = 0;
        blt->Top = ((s32) (dist / 528000.0f) % 10) * w;
        if ((s32) (dist / 52800.0f) % 10 == 9 && (s32) (dist / 5280.0f) % 10 == 9 && itenths % 10 == 9) {
            blt->Top = blt->Top + frac * w;
        }
        break;
    case 1:
        w = blt->Width / 2;
        blt->X = D_80115AE8[D_80151AD0 - 1][slot].x - w;
        blt->Y = D_80115AE8[D_80151AD0 - 1][slot].y;
        blt->Left = 0;
        blt->Top = ((s32) (dist / 52800.0f) % 10) * w;
        if ((s32) (dist / 5280.0f) % 10 == 9 && itenths % 10 == 9) {
            blt->Top = blt->Top + frac * w;
        }
        break;
    case 2:
        blt->X = D_80115AE8[D_80151AD0 - 1][slot].x;
        blt->Y = D_80115AE8[D_80151AD0 - 1][slot].y;
        blt->Left = 0;
        w = blt->Width / 2;
        blt->Top = ((s32) (dist / 5280.0f) % 10) * w;
        if (itenths % 10 == 9) {
            blt->Top = blt->Top + frac * w;
        }
        break;
    case 3:
        w = blt->Width / 2;
        blt->Left = w;
        blt->Top = w * (itenths % 10 + tenths - itenths);
        blt->X = D_80115AE8[D_80151AD0 - 1][slot].x + w;
        blt->Y = D_80115AE8[D_80151AD0 - 1][slot].y;
        break;
    default:
        w = blt->Width / 2;
        break;
    }
    blt->Right = blt->Left + w - 1;
    blt->Bot = blt->Top + w - 1;
    Input_ApplyPadConfig(blt);
    return 1;
}
