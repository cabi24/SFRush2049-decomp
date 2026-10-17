typedef signed char s8;typedef unsigned char u8;typedef signed short s16;typedef unsigned short u16;typedef signed int s32;typedef unsigned int u32;
typedef struct Blit Blit;
typedef s32 (*PaneAnimation)(Blit *);
struct Blit {
    const char *name; void *palette; void *info; u16 texIndex; s16 x,y; u16 state;
    s16 width,height; u8 alpha,flip; s8 hide,init; s16 top,bot,left,right,color; u16 pad26;
    PaneAnimation animFunc; u32 animID; s32 animDTA; u16 blIdx,pad36;void *data;Blit *child;
};
typedef struct Pane { s8 used,perm;u8 blackColor[4],grayColor[4];Blit *blits[5]; } Pane;
extern Pane D_80116DE4[];
extern Blit *func_800B3704();
extern void Input_ApplyPadConfig(Blit *);
void crowd_cheer_play(Pane *,s32,s32,s32,s32);
Pane *ambient_sound_set(s32 x1,s32 y1,s32 x2,s32 y2,s32 alpha,PaneAnimation animFunc,u32 animID,s32 permanent) {
    s32 i;
    Pane *pane;
    for(i=0,pane=D_80116DE4;i<16;i++,pane++) { if(!pane->used)break; }
    if(i>=16)return pane;
    if(x1>x2){x1^=x2;x2^=x1;x1^=x2;}
    if(y1>y2){y1^=y2;y2^=y1;y1^=y2;}
    pane->blackColor[0]=0;
    pane->blackColor[1]=0;
    pane->blackColor[2]=0x10;
    pane->blackColor[3]=alpha;
    pane->grayColor[0]=0;
    pane->grayColor[1]=0;
    pane->grayColor[2]=0xc0;
    pane->grayColor[3]=alpha;
    pane->perm=permanent;
    pane->used=1;
    for(i=0;i<5;i++) {
        pane->blits[i]=func_800B3704(0,x1,y1,0);
        pane->blits[i]->alpha=alpha;
    pane->blits[i]->palette=pane->blackColor;
        pane->blits[i]->animFunc=animFunc;
    pane->blits[i]->animID=animID;
    pane->blits[i]->animDTA=0;
    }
    pane->blits[0]->animDTA=(s32)pane;
    pane->blits[0]->palette=pane->grayColor;
    pane->blits[1]->palette=pane->grayColor;
    pane->blits[3]->palette=pane->grayColor;
    pane->blits[4]->palette=pane->grayColor;
    crowd_cheer_play(pane,x1,y1,x2,y2);
    /* Native body has no GC-only IgnoreDrawFirstBlit loop. */
    return pane;
}
void tournament_unlock_check(Pane *pane,u8 *paneColor,u8 *borderColor) {
    s32 i;
    pane->blackColor[0]=paneColor[0];
    pane->blackColor[1]=paneColor[1];
    pane->blackColor[2]=paneColor[2];
    pane->blackColor[3]=paneColor[3];
    pane->grayColor[0]=borderColor[0];
    pane->grayColor[1]=borderColor[1];
    pane->grayColor[2]=borderColor[2];
    pane->grayColor[3]=borderColor[3];
    for(i=0;i<5;i++) {
        if(i==2){pane->blits[i]->palette=pane->blackColor;
    pane->blits[i]->alpha=paneColor[3];}
        else {pane->blits[i]->palette=pane->grayColor;
    pane->blits[i]->alpha=borderColor[3];}
        Input_ApplyPadConfig(pane->blits[i]);
    }
}
void crowd_cheer_play(Pane *group,s32 x1,s32 y1,s32 x2,s32 y2)
{
    s32 i;
    if(x2<x1) { x1^=x2; x2^=x1; x1^=x2; }
    if(y2<y1) { y1^=y2; y2^=y1; y1^=y2; }
    for(i=0;i<5;i++) {
        group->blits[i]->x=x1;
        group->blits[i]->y=y1;
        group->blits[i]->width=x2-x1;
        group->blits[i]->height=y2-y1;
    }
    group->blits[0]->height=1;
    group->blits[1]->width=1;
    group->blits[1]->y++;
    group->blits[1]->height-=2;
    group->blits[2]->x++;
    group->blits[2]->y++;
    group->blits[2]->width-=2;
    group->blits[2]->height-=2;
    group->blits[3]->x+=group->blits[3]->width-1;
    group->blits[3]->width=1;
    group->blits[3]->y++;
    group->blits[3]->height-=2;
    group->blits[4]->y+=group->blits[4]->height-1;
    group->blits[4]->height=1;
    for(i=0;i<5;i++) {
        Input_ApplyPadConfig(group->blits[i]);
    }
}

extern void sound_stop(Blit *);
void sound_handles_clear(s32 permanent)
{
    s32 i;
    s32 j;
    for (i = 0; i < 16; i++) {
        if (D_80116DE4[i].used && (permanent || !D_80116DE4[i].perm)) {
            for (j = 0; j < 5; j++) {
                if (D_80116DE4[i].blits[j]) {
                    sound_stop(D_80116DE4[i].blits[j]);
                    D_80116DE4[i].blits[j] = 0;
                }
            }
            D_80116DE4[i].used = 0;
        }
    }
}
