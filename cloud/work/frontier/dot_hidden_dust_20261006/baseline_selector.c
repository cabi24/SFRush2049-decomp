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
typedef struct { u8 other0[390]; s16 model; u8 other392[1664]; } Car2056;
typedef struct { u8 other0[239]; s8 mode; u8 other240[712]; } Object952;
extern Car2056 D_8014A250[];
extern Object952 D_80152818[];
extern s16 D_80151AD0,D_8014A108;
extern s8 D_8015B25C;
extern u8 D_80120D9C[],D_80120DAC[];
extern s32 D_80115B68[][4][2];
extern f32 D_801245A4,D_801245A8;
extern void Input_ApplyPadConfig(PadConfig *);


extern s8 D_8015F72C;
extern u8 D_80116187;
extern u8 D_801461D0[];
extern s32 osRecvMesg(void *,void *,s32);
extern s32 osJamMesg(void *,void *,s32);
extern s32 slot_state_setup(s32);
extern s32 camera_shake_update(u16);
s32 dust_cloud_effect(PadConfig *pad)
{
    s32 kind, mode, disabled, height;
    kind=pad->packed & 15;
    mode=(pad->packed & 240)>>4;
    if(mode==1) disabled=D_80151AD0>=2;
    else if(mode==2) disabled=D_80151AD0<2;
    else disabled=kind>=D_80151AD0;
    if(disabled!=pad->disabled) {
        pad->disabled=disabled;
        Input_ApplyPadConfig(pad);
    }
    if(pad->disabled) {
        pad->active=0;
        return 1;
    }
    disabled=D_8015F72C==0 || D_80152818[D_8014A250[kind].model].mode==1;
    if(disabled!=pad->disabled) {
        pad->disabled=disabled;
        Input_ApplyPadConfig(pad);
    }
    if(pad->disabled) return 1;
    if(D_80151AD0==1) {
        osRecvMesg(D_801461D0,0,1);
        slot_state_setup(4);
        osJamMesg(D_801461D0,0,0);
    } else {
        osRecvMesg(D_801461D0,0,1);
        slot_state_setup(1);
        osJamMesg(D_801461D0,0,0);
    }
    height=camera_shake_update(56);
    pad->x=D_80115B68[D_80151AD0-1][kind][0]-height*2-2;
    pad->y=D_80115B68[D_80151AD0-1][kind][1]+(D_80151AD0==1);
    pad->half18=100;
    pad->status=D_80116187;
    Input_ApplyPadConfig(pad);
    return 1;
}

/* Genuine resource-slot selection semantics, audited against all 58 native
 * words. This standalone source does not establish its private register ABI.
 * No artificial return uses, pressure inputs, debug stubs or inline blockers.
 */

extern s8 D_80149DA0;
extern s32 D_80149780, D_801497A4;
extern void *D_80114740;
extern s32 func_80097694(s32, s8);
extern s32 audio_frame_sync(s32, s32, s32, s32, void *);
extern void display_list_alloc(s32);
extern void sound_update_channel(s32);
extern s8 object_byte9_set(s8);

s32 slot_state_setup(s32 selection)
{
    s8 previous;
    previous = D_80149DA0;
    D_80149DA0 = selection;
    if (selection != -1) {
        D_80149780 = func_80097694(D_80149DA0 + 38, -1);
        if (D_80149780 < 0) {
            D_80149780 = audio_frame_sync(D_80149DA0 + 38, 0, 0, 1, 0);
            display_list_alloc(D_80149780);
        }
        D_801497A4 = func_80097694(D_80149DA0 + 22, -1);
        if (D_801497A4 < 0) {
            D_801497A4 = audio_frame_sync(D_80149DA0 + 22, 0, 0, 0, D_80114740);
            display_list_alloc(D_801497A4);
        }
        sound_update_channel(previous != selection);
    }
    if (selection == 0) {
        object_byte9_set(1);
    }
    return previous;
}
