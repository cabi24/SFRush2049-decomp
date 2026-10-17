#include <stdio.h>
#include <stdint.h>
s16 D_80151AD0;u32 D_801174B4;s8 D_8015723C;u8 D_80116187;
#ifndef OWNED_ELAPSED_TABLE
Position8 D_80115C88[4]={{272,142},{272,142},{240,197},{160,134}};
#endif
static u32 seed=0x7045c8f1u;static uint64_t hash;static u32 events;static s32 font;static u8 nonProp;
static u32 rnd(void){seed=seed*1664525u+1013904223u;return seed;}
static void record(u32 x){hash^=x;hash*=1099511628211ULL;events++;}
static s32 sentinel(Blit *b){return b!=0;}
void Input_ApplyPadConfig(Blit *b){record(1);record((s32)b->x);record((s32)b->y);record(b->state);record(b->alpha);record((s32)b->hide);record(b->animFunc!=0);record(b->animID);}
s32 object_create(s32 x){s32 old=font;font=x;record(2);record(x);return old;}
u32 camera_shake_update(u16 c){record(3);record(c);return nonProp?11:(c==':'?3:7)+font;}
s8 object_byte9_set(s8 x){s8 old=(s8)nonProp;nonProp=(u8)x;record(4);record((s32)x);return old;}
int main(void){u32 i;for(i=0;i<8192;i++){
 u32 r=rnd();Blit b={0};D_80151AD0=1+((r>>8)&3);D_801174B4=(r&3)?0x400000:0;if(r&4)D_801174B4|=8;
 D_8015723C=(r&7)!=0;D_80116187=r>>24;b.x=(s16)(r>>4);b.y=(s16)(r>>16);b.state=17;b.alpha=71;b.hide=(r>>12)&1;b.animFunc=sentinel;
 b.animID=(r>>20)&31;font=-1;nonProp=(u8)(r>>15);
 hash=1469598103934665603ULL;events=0;record(checkpoint_hit(&b));record((s32)b.x);record((s32)b.y);record(b.state);record(b.alpha);record((s32)b.hide);record(b.animFunc!=0);record(nonProp);
 printf("%u %016llx %u\n",i,(unsigned long long)hash,events);
}return 0;}
