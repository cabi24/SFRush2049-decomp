/* Actual complete submitted C, reachable panel only; external draw services are
 * deterministic doubles. Native private-register execution is not simulated. */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>
#include <stdint.h>
#include "groups/runtime_a_stats_menu_20261006/group.c"
Language countdown_state;
Profile **D_803BA800;
s8 D_803BA858;
u8 D_80151410[12*64];
FiveParts *D_803B6530;
u8 D_803B8B38[]="%d",D_803B8B3C[]="%d",D_803B8B40[]="%d",D_803B8B44[]="%d";
u8 D_803B8B48[]="%d",D_803B8B4C[]="%d",D_803B8B50[]="%d/8",D_803B8B58[]="%d/8";
static int line,draws,formats,counts,heights,height,profile;
static u32 words[3]; static u16 short_value,flags;
static const char *labels[]={"row0","row1","row2","row3","row4","row5","row6","row7"};
static void check(int ok) {if(!ok){fprintf(stderr,"panel assertion at line %d draws %d\n",line,draws);exit(1);}}
s32 object_create(s32 font) {check(font==11);return 0;}
void dispatch_handler(s32 mode) {check(mode==1);}
s16 sound_pitch_diff_calc(void *text,s16 right) {check(right==186);return right-(s16)strlen(text);}
s16 object_bytes_sum_global(void) {heights++;return height;}
void fcvt_wrapper(u8 *out,const u8 *format,...) {
    va_list ap;u32 raw;int value,n;va_start(ap,format);raw=va_arg(ap,u32);va_end(ap);value=(s32)raw;
    n=sprintf((char *)out,(const char *)format,value);check(n>=0 && n+1<=48);formats++;
}
s32 func_800B78A4(u32 x,u8 n) {
    s32 c=0;check(n==16);check(x==(counts ? (flags&0xff00U):(flags&255U)));counts++;
    while(n!=0 && x!=0) {if(x&1)c++;n--;x>>=1;}return c;
}
static int pop(unsigned x) {int k=0;while(x){k+=x&1;x>>=1;}return k;}
void state_utility(s16 x,s16 y,void *text) {
    int row=draws/2;char expected[48];int index=row<4?row:row+2;
    check(y==90+row*height);
    if(!(draws&1)) {check(x==186-(int)strlen(labels[index]));check(!strcmp(text,labels[index]));}
    else {int val=row<3?(s32)words[row]:row==3?short_value:pop(row==4?(flags&255):(flags>>8));
        sprintf(expected,row<4?"%d":"%d/8",val);check(x==194);check(!strcmp(text,expected));}
    draws++;
}
void crowd_cheer_play(FiveParts *box,s32 left,s32 top,s32 right,s32 bottom) {
    int rows=profile?6:4;check(box==D_803B6530 && left==80 && top==86 && right==240 && bottom==94+height*rows);
}
int main(void) {
    static union {u32 align;u8 bytes[12+12*64];} storage;
    static u16 indices[58];static u8 *indexed[8];
    Profile owner,*owner_ptr=&owner;u8 *stats=storage.bytes;u32 rng=1;int mode,selector,j,k,total=0;
    owner.stats=&stats;countdown_state.indices=indices;countdown_state.indexed=indexed;
    for(k=0;k<8;k++)indexed[k]=(u8 *)labels[k];
    for(mode=0;mode<2;mode++)for(selector=0;selector<12;selector++)for(j=0;j<1000;j++) {
        u8 *data;u8 before[64];profile=mode;D_803BA800=mode?&owner_ptr:0;D_803BA858=mode?selector:selector+20;
        data=mode?storage.bytes+12+selector*64:D_80151410+selector*64;
        for(k=0;k<3;k++){rng=rng*1664525U+1013904223U;words[k]=j<4?(u32[]){0,0x7fffffffU,0x80000000U,0xffffffffU}[j]:rng;memcpy(data+4+k*4,&words[k],4);}
        short_value=(u16)(j*67);flags=(u16)(j*61);memcpy(data+16,&short_value,2);memcpy(data+60,&flags,2);memcpy(before,data,64);
        height=4+j%17;draws=formats=counts=heights=0;line=total;
        func_803ACA94();check(draws==(mode?12:8) && formats==(mode?6:4) && counts==(mode?2:0) && heights==(mode?7:5));
        check(!memcmp(before,data,64));check(D_803BA800==(mode?&owner_ptr:0));check(D_803BA858==(mode?selector:selector+20));total++;
    }
    printf("{\"cases\":%d,\"submitted_group_source\":true,\"external_draw_doubles\":true,\"real_bit_counter_logic\":true}\n",total);return 0;
}
