/* flags: -g0 -O3 -mips2 -G 0 -non_shared */
/* N64 SelectBlit, 0x800FE5B0. Adapted from the arcade LIB/blit.c
 * SelectBlit control/data flow, using native N64 fields and coordinate direction.
 * N64 forces a zero sections-per-row quotient to one and leaves the destination
 * width and height unchanged. Native Top/Bot coordinates increase downwards.
 * Source lead: historicalsource/rushtherock at 845329d7b36f5a384c5625ed9a0aef584ab46139,
 * LIB/blit.c SelectBlit (Atari Games arcade source). The layouts below are the
 * observed N64 prefixes; sizeof(Blit) is not asserted to be the full record size.
 * No padding locals, artificial reads, or whole-program stand-ins are used.
 */
typedef unsigned char u8;
typedef signed char s8;
typedef unsigned short u16;
typedef signed short s16;
typedef signed int s32;
typedef struct TexDef {
    char name[16];
    u16 Width, Height;
    u8 other[16];
} TexDef;
typedef struct Blit {
    char *Name;
    void *Image;
    TexDef *Info;
    s16 TexIndex, X, Y;
    u16 State;
    s16 Width, Height;
    u8 Alpha, Flip;
    s8 Hide;
    u8 Init;
    s16 Top, Bot, Left, Right, Color;
} Blit;
extern void func_800EF5B0(Blit *, char *, s32);
extern void Input_ApplyPadConfig(Blit *);
void stat_race_update(Blit *blt, int index, int hSize, int vSize)
{
    int row, col, secPerRow;

    if (!blt)
        return;
    if (!blt->Info || !blt->Info->Width) {
        func_800EF5B0(blt, blt->Name, 1);
        if (!blt->Info)
            return;
    }
    secPerRow = blt->Info->Width / hSize;
    if (!secPerRow)
        secPerRow = 1;
    row = index / secPerRow;
    col = index % secPerRow;
    blt->Top = row * vSize;
    blt->Bot = blt->Top + vSize - 1;
    if (blt->Flip) {
        col = secPerRow - col - 1;
        blt->Left = (blt->Info->Width % hSize) + col * hSize;
        blt->Right = blt->Left + hSize - 1;
    } else {
        blt->Left = col * hSize;
        blt->Right = blt->Left + hSize - 1;
    }
    Input_ApplyPadConfig(blt);
}
