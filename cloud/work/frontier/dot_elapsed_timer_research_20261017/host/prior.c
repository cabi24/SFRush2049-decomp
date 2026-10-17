typedef signed char s8;typedef unsigned char u8;typedef signed short s16;typedef unsigned short u16;typedef signed int s32;typedef unsigned int u32;typedef float f32;
typedef struct Blit Blit;
struct Blit { const char *name;void *palette;void *info;u16 texIndex;s16 x,y;u16 state;
 s16 width,height;u8 alpha,flip;s8 hide,init;s16 top,bot,left,right,color;u16 pad26;
 s32 (*animFunc)(Blit *);u32 animID;s32 animDTA;u16 blIdx,pad36;void *data;Blit *child; };
extern void Input_ApplyPadConfig(Blit *);
extern s16 D_80151AD0;extern u32 D_801174B4;extern s8 D_8015723C;extern u8 D_80116187;
typedef struct Position8 {s32 x,y;} Position8;extern Position8 D_80115C88[4];
s32 object_create(s32);u32 camera_shake_update(u16);s8 object_byte9_set(s8);
s32 checkpoint_hit(Blit *pad)
{
    s32 disabled,width56,width58;
    s8 old;
    disabled=(pad->animID==1 && D_80151AD0>=2) || (pad->animID==2 && D_80151AD0<2);
    if(disabled!=pad->hide) {
        pad->hide=disabled;
        Input_ApplyPadConfig(pad);
    }
    if(pad->hide) {
        pad->animFunc=0;
        return 1;
    }
    disabled=(D_801174B4&8)!=0 || (D_801174B4&0x400000)==0 || D_8015723C==0;
    if(disabled!=pad->hide) {
        pad->hide=disabled;
        Input_ApplyPadConfig(pad);
    }
    if(pad->hide)return 1;
    if(D_80151AD0==1) {
        object_create(1);
    } else {
        object_create(2);
    }
    width56=camera_shake_update(56)+1;
    old=object_byte9_set(0);
    width58=camera_shake_update(58)+1;
    object_byte9_set(old);
    pad->x=D_80115C88[D_80151AD0-1].x-(width56*7+width58*2)/2;
    pad->y=D_80115C88[D_80151AD0-1].y-1;
    pad->state=100;
    pad->alpha=D_80116187;
    Input_ApplyPadConfig(pad);
    return 1;
}
