/* flags: -g0 -O3 -mips2 -G 0 -non_shared */
/* Research NONMATCH 14/57. N64 NewBlit, from rushtherock LIB/blit.c.
 * Authentic u32 AnimID / s32 AnimDTA restores separate -1 materializations.
 * This source has three formals and makes no matching or coverage claim. */
typedef signed char s8;
typedef unsigned char u8;
typedef signed short s16;
typedef unsigned short u16;
typedef signed int s32;
typedef unsigned int u32;
typedef struct Blit Blit;
struct Blit {
    const char *Name;
    void *Image;
    void *Info;
    u16 TexIndex;
    s16 X, Y;
    u16 Z;
    s16 Width, Height;
    u8 Alpha, Flip;
    s8 Hide, Init;
    s16 Top, Bot, Left, Right, color;
    u16 reserved;
    s32 (*AnimFunc)(Blit *);
    u32 AnimID;
    s32 AnimDTA;
    u16 BLIdx;
    u16 reserved36;
    void *data;
    Blit *child;
};
extern s32 D_80149788;
extern Blit *D_80149450[];
extern void collision_sound_play(Blit *);
extern s32 func_800A79F4(u16, void *, void *, s32, s32, s32, s32);
extern void Input_ApplyPadConfig(Blit *);
Blit *func_800B3704(const char *name, int x, int y)
{
    Blit *blit;
    if (D_80149788 >= 200) return (Blit *)0;
    blit = D_80149450[D_80149788];
    D_80149788++;
    blit->Name = name;
    blit->X = x;
    blit->Y = y;
    blit->Hide = 0;
    blit->Flip = 0;
    blit->Init = 1;
    blit->child = (Blit *)0;
    blit->AnimFunc = 0;
    blit->AnimID = -1;
    blit->AnimDTA = -1;
    blit->data = (void *)0;
    collision_sound_play(blit);
    blit->BLIdx = func_800A79F4(blit->TexIndex, 0, 0, x, y, -1, -1);
    Input_ApplyPadConfig(blit);
    return blit;
}
