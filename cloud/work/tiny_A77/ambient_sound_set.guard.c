/* flags: -g0 -O3 -mips2 -G 0 -non_shared */
typedef signed char s8;
typedef unsigned char u8;
typedef signed short s16;
typedef unsigned short u16;
typedef signed int s32;
typedef unsigned int u32;
typedef struct {
    void *data; u8 *bytes4; s32 word8; u16 half12; s16 x,y; u16 half18;
    s16 width,height;u8 status,flag; s8 disabled; u8 other27;
    s16 left,bottom,right,top; u8 other36[4];s32 word40,word44;void *link48;
} PadConfig;
typedef struct { s8 active; u8 own,a,b,c,d,e,f,g,h;u8 other10[2];PadConfig *part[5]; } FiveParts;
extern FiveParts D_80116DE4[];
extern PadConfig *func_800B3704();
extern void crowd_cheer_play(FiveParts *,s32,s32,s32,s32);
FiveParts *ambient_sound_set(s32 x1,s32 y1,s32 x2,s32 y2,s32 style,s32 word40,s32 word44,s32 own)
{
    FiveParts *group=D_80116DE4;
    s32 i;
    for(i=0;i<16;i++,group++) {
        if(!group->active) break;
    }
    if(i>=16) return group;
    if(x2<x1) { x1^=x2; x2^=x1; x1^=x2; }
    if(y2<y1) { y1^=y2; y2^=y1; y1^=y2; }
    group->a=0;group->b=0;group->c=16;group->d=style;
    group->e=0;group->f=0;group->g=192;group->h=style;
    group->active=1;group->own=own;
    for(i=0;i<5;i++) {
        group->part[i]=func_800B3704((void *)0,x1,y1,0);
        group->part[i]->status=style;
        group->part[i]->bytes4=&group->a;
        group->part[i]->word40=word40;
        group->part[i]->word44=word44;
        group->part[i]->link48=0;
    }
    group->part[0]->link48=group;
    group->part[0]->bytes4=&group->e;
    group->part[1]->bytes4=&group->e;
    group->part[3]->bytes4=&group->e;
    group->part[4]->bytes4=&group->e;
    crowd_cheer_play(group,x1,y1,x2,y2);
    return group;
}
