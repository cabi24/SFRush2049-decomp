/* flags: -g0 -O2 -mips2 -G 0 -non_shared */
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
typedef struct { u8 other[12]; PadConfig *part[5]; } FiveParts;
extern void Input_ApplyPadConfig(PadConfig *);
void crowd_cheer_play(FiveParts *group,s32 x1,s32 y1,s32 x2,s32 y2)
{
    s32 i,width,height;
    PadConfig **part;
    if(x2<x1) { x1^=x2; x2^=x1; x1^=x2; }
    width=x2-x1;
    if(y2<y1) { y1^=y2; y2^=y1; y1^=y2; }
    height=y2-y1;
    for(i=0;i<5;i++) {
        group->part[i]->x=x1;
        group->part[i]->y=y1;
        group->part[i]->width=width;
        group->part[i]->height=height;
    }
    group->part[0]->height=1;
    group->part[1]->width=1;
    group->part[1]->y++;
    group->part[1]->height-=2;
    group->part[2]->x++;
    group->part[2]->y++;
    group->part[2]->width-=2;
    group->part[2]->height-=2;
    group->part[3]->x+=group->part[3]->width-1;
    group->part[3]->width=1;
    group->part[3]->y++;
    group->part[3]->height-=2;
    group->part[4]->y+=group->part[4]->height-1;
    group->part[4]->height=1;
    for(i=0;i<5;i++) {
        Input_ApplyPadConfig(group->part[i]);
    }
}
