/* flags: -g0 -O3 -mips2 -G 0 -non_shared */
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
typedef struct { u8 other0[1990]; s16 model2; u8 other1992[64]; } Car2056;
typedef struct { u8 other0[239]; s8 mode; u8 other240[712]; } Object952;
extern Car2056 D_8014A250[];
extern Object952 D_80152818[];
extern s16 D_80151AD0,D_8014A108;
extern s8 D_8015B25C;
extern u8 D_80120D9C[],D_80120DAC[];
extern s32 D_80115A68[][4][2];
extern f32 D_801245A4,D_801245A8;
extern void Input_ApplyPadConfig(PadConfig *);


extern s8 D_8015F72C;
extern u8 D_80116187;
extern u8 D_801461D0[];
extern s32 osRecvMesg(void *,void *,s32);
extern s32 osJamMesg(void *,void *,s32);
extern s32 slot_state_setup(s32);
extern s32 osSendMesg(void *,void *,s32);
static void font_lock(void) {
    osRecvMesg(D_801461D0, 0, 1);
}

static void font_unlock(void) {
    osSendMesg(D_801461D0, 0, 0);
}

static s32 font_select(s32 sel) {
    s32 old;
    font_lock();
    old = slot_state_setup(sel);
    font_unlock();
    return old;
}

extern s32 camera_shake_update(u16);
s32 dust_cloud_effect(PadConfig *pad)
{
    s32 kind, mode, disabled, height;
    kind=pad->packed & 15;
    mode=(pad->packed & 240)>>4;
    disabled=(mode==1 && D_80151AD0>=2) || (mode==2 && D_80151AD0<2) || kind>=D_80151AD0;
    if(disabled!=pad->disabled) {
        pad->disabled=disabled;
        Input_ApplyPadConfig(pad);
    }
    if(pad->disabled) {
        pad->active=0;
        return 1;
    }
    disabled=D_8015F72C==0 || D_80152818[D_8014A250[kind].model2].mode==1;
    if(disabled!=pad->disabled) {
        pad->disabled=disabled;
        Input_ApplyPadConfig(pad);
    }
    if(pad->disabled) return 1;
    if(D_80151AD0==1) {
        font_select(4);
    } else {
        font_select(1);
    }
    height=camera_shake_update(56);
    pad->x=D_80115A68[D_80151AD0-1][kind][0]-height*2-2;
    pad->y=D_80115A68[D_80151AD0-1][kind][1]+((D_80151AD0==1) ? 1 : 0);
    pad->half18=100;
    pad->status=D_80116187;
    Input_ApplyPadConfig(pad);
    return 1;
}

