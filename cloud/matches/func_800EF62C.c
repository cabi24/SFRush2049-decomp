/* flags: -g0 -O3 -mips2 -G 0 -non_shared */
/*
 * N64 per-player HUD bar animation. Packed AnimID chooses player (bits 4..7)
 * and crop kind (bits 0..3). Hide inactive/unavailable players, select the
 * split-screen texture, position it, and crop the bar from a signed car value.
 * The native crop assignment order is Right, Top, Bot. Reordering the final
 * independent stores changes IDO's temporary register cycle.
 *
 * Shared source contract: arcade game/hud.c Hidden and LIB/blit.h, revision
 * 845329d7b36f5a384c5625ed9a0aef584ab46139. The complete N64 bar callback and
 * per-player tables are platform-specific. Object prefixes/strides below are
 * observed native layouts, not fabricated local storage. No padding locals,
 * dead reads, stand-ins, altered call contracts, or keep settings are used.
 */
typedef signed char s8;
typedef unsigned char u8;
typedef signed short s16;
typedef unsigned short u16;
typedef signed int s32;
typedef unsigned int u32;
typedef float f32;
typedef struct Blit {
    const char *Name;
    void *image;
    void *Info;
    s16 TexIndex;
    s16 X, Y;
    u16 Z;
    s16 Width, Height;
    u8 Alpha, Flip;
    s8 Hide;
    u8 Init;
    s16 Top, Bot, Left, Right;
    s16 color;
    u16 unknown26;
    s32 (*AnimFunc)(struct Blit *);
    u32 AnimID;
} Blit;
typedef struct { u8 other0[10]; s8 flag; u8 other11[1979]; s16 index; u8 other1992[8]; s16 value; u8 other2002[54]; } Car2056;
typedef struct { u8 other0[239]; s8 mode; u8 other240[712]; } Object952;
extern Car2056 D_8014A250[];
extern Object952 D_80152818[];
extern s16 D_80151AD0,D_8014A108;
extern s8 D_8015B25C;
extern char D_80120D9C[],D_80120DAC[];
extern s32 D_80115B68[][4][2];

extern void Input_ApplyPadConfig(Blit *);
extern void func_800EF5B0(Blit *,const char *,s32);
s32 func_800EF62C(Blit *blt)
{
    s32 slot, kind, hide;
    f32 amount;
    slot=(blt->AnimID & 0xf0)>>4;
    kind=blt->AnimID & 0xf;
    if(slot>=D_80151AD0) {
        if(slot>=D_8014A108) blt->AnimFunc=0;
        if (blt->Hide != 1) {
            blt->Hide = 1;
            Input_ApplyPadConfig(blt);
        }
        return 1;
    }
    hide = D_8015B25C == 0 || D_8014A250[slot].flag != 0 ||
           D_80152818[D_8014A250[slot].index].mode == 1;
    if (hide != blt->Hide) {
        blt->Hide = hide;
        Input_ApplyPadConfig(blt);
    }
    if (blt->Hide) return 1;
    if(D_80151AD0==2) func_800EF5B0(blt,D_80120D9C,0);
    else if(D_80151AD0>=3) func_800EF5B0(blt,D_80120DAC,0);
    blt->X=D_80115B68[D_80151AD0-1][slot][0]-blt->Width/2;
    blt->Y=D_80115B68[D_80151AD0-1][slot][1];
    if(kind==0) blt->Bot=blt->Height/2-1;
    else if(kind==1) {
        amount=(D_8014A250[slot].value*1.35f)*0.0001f;
        if(amount<0.0f) amount=-amount;
        if(amount>1.0f) amount=1.0f;
        blt->Right=(s32)((blt->Width-1)*amount);
        blt->Top=blt->Height/2;
        blt->Bot=blt->Height-1;
    }
    blt->Alpha=254;
    Input_ApplyPadConfig(blt);
    return 1;
}
