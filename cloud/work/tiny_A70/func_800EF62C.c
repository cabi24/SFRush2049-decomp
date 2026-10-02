/* IDO flags: -g0 -O2 -mips2 -G 0 -non_shared */
typedef signed char s8;
typedef unsigned char u8;
typedef signed short s16;
typedef unsigned short u16;
typedef signed int s32;
typedef unsigned int u32;
typedef float f32;
typedef struct {
    void *data; s32 word4,word8; u16 half12; s16 x,y; u16 half18;
    s16 width,height; u8 status,flag; s8 disabled; u8 other27;
    s16 left,bottom,right,top; u8 other36[4]; s32 active; u32 packed;
} PadConfig;
typedef struct { u8 other0[10]; s8 flag; u8 other11[1979]; s16 index; u8 other1992[8]; s16 value; u8 other2002[54]; } Car2056;
typedef struct { u8 other0[239]; s8 mode; u8 other240[712]; } Object952;
extern Car2056 D_8014A250[];
extern Object952 D_80152818[];
extern s16 D_80151AD0,D_8014A108;
extern s8 D_8015B25C;
extern u8 D_80120D9C[],D_80120DAC[];
extern s32 D_80115B68[][4][2];
extern f32 D_801245A4,D_801245A8;
extern void Input_ApplyPadConfig(PadConfig *);
extern void func_800EF5B0(PadConfig *,void *,s32);
s32 func_800EF62C(PadConfig *pad)
{
    s32 slot,kind,disabled;
    f32 amount;
    slot=(pad->packed & 0xf0)>>4;
    kind=pad->packed & 0xf;
    if(slot>=D_80151AD0) {
        if(slot>=D_8014A108) pad->active=0;
        if(pad->disabled!=1) {
            pad->disabled=1;
            Input_ApplyPadConfig(pad);
        }
        return 1;
    }
    disabled=D_8015B25C==0 || D_8014A250[slot].flag!=0 || D_80152818[D_8014A250[slot].index].mode==1;
    if(disabled!=pad->disabled) {
        pad->disabled=disabled;
        Input_ApplyPadConfig(pad);
    }
    if(pad->disabled) return 1;
    if(D_80151AD0==2) func_800EF5B0(pad,D_80120D9C,0);
    else if(D_80151AD0>=3) func_800EF5B0(pad,D_80120DAC,0);
    pad->x=D_80115B68[D_80151AD0-1][slot][0]-pad->width/2;
    pad->y=D_80115B68[D_80151AD0-1][slot][1];
    if(kind==0) pad->bottom=pad->height/2-1;
    else if(kind==1) {
        amount=(D_8014A250[slot].value*D_801245A4)*D_801245A8;
        if(amount<0.0f) amount=-amount;
        if(amount>1.0f) amount=1.0f;
        pad->bottom=pad->height-1;
        pad->top=(s32)((pad->width-1)*amount);
        pad->left=pad->height/2;
    }
    pad->status=254;
    Input_ApplyPadConfig(pad);
    return 1;
}
