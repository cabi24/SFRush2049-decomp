/* Actual candidate C on a logical host fixture. These services model the
 * existing O32 argument-bit contract; they do not execute retail helpers. */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>
#include <stdint.h>
#include "groups/runtime_a_stats_menu_20261006/group.c"
Language countdown_state;
Profile **D_803BA800;
s8 D_80146111;
FiveParts *D_803B6530;
u8 D_803B8B9C[]="%01d.%01d",D_803B8BA8[]="%01d.%01d";
u8 D_803B8BB4[]="%d",D_803B8BB8[]="%d";
u8 D_803B8BBC[]="%d/%d",D_803B8BC4[]="%d/%d",D_803B8BCC[]="%d/%d",D_803B8BD4[]="%d/%d";
static int current_case,height,draws,counts,heights,formats,setups,fonts,dispatches,boxes;
static char labels[8][8],expected[7][48];
static u32 expected_masks[20];
static void check(int ok){if(!ok){fprintf(stderr,"aggregate case %d draw %d count %d failure\n",current_case,draws,counts);exit(1);}}
void func_800B669C(u32 a,u32 b){check(a==0&&b==3);setups++;}
s32 object_create(s32 font){check(font==11);fonts++;return 0;}
void dispatch_handler(s32 mode){check(mode==1);dispatches++;}
s16 sound_pitch_diff_calc(void *text,s16 right){check(right==176);return right-(s16)strlen(text);}
s16 object_bytes_sum_global(void){heights++;return height;}
void crowd_cheer_play(FiveParts *box,s32 left,s32 top,s32 right,s32 bottom){check(box==D_803B6530&&left==12&&top==86&&right==308&&bottom==94+height*7);boxes++;}
void fcvt_wrapper(u8 *out,const u8 *fmt,...){
    va_list ap;int n;va_start(ap,fmt);
    if(fmt==D_803B8B9C||fmt==D_803B8BA8){u32 a=va_arg(ap,u32),b=va_arg(ap,u32);n=sprintf((char *)out,"%01d.%01d",(int32_t)a,(int32_t)b);}
    else if(fmt==D_803B8BB4){u32 a=va_arg(ap,u32);n=sprintf((char *)out,"%d",(int32_t)a);}
    else if(fmt==D_803B8BB8){int a=va_arg(ap,int);n=sprintf((char *)out,"%d",a);}
    else {int a=va_arg(ap,int),b=va_arg(ap,int);check(fmt==D_803B8BBC||fmt==D_803B8BC4||fmt==D_803B8BCC||fmt==D_803B8BD4);n=sprintf((char *)out,"%d/%d",a,b);}
    va_end(ap);check(n>=0&&n+1<=12);formats++;
}
s32 func_800B78A4(u32 x,u8 n){s32 count=0;check(counts<20&&n==16&&x==expected_masks[counts]);counts++;while(n--){count+=x&1;x>>=1;}return count;}
void state_utility(s16 x,s16 y,void *text){int row=draws/2;check(row<7&&y==90+row*height);if(!(draws&1)){int label=row==0&&D_80146111?7:row;check(x==176-(int)strlen(labels[label]));check(!strcmp(text,labels[label]));}else{check(x==184);check(!strcmp(text,expected[row]));}draws++;}
static int pop(unsigned v){int n=0;while(v){n+=v&1;v>>=1;}return n;}
int main(void){
    static union {u32 align;u8 bytes[1792];} storage;
    static u16 indices[40];static u8 *indexed[15];
    Profile owner,*owner_ptr=&owner;u8 *stats=storage.bytes;int mode,units,trial,k,total_cases=0;u32 rng=7;
    owner.stats=&stats;countdown_state.indices=indices;countdown_state.indexed=indexed;
    for(k=0;k<8;k++){sprintf(labels[k],"row%d",k);indexed[k]=(u8 *)labels[k];}indices[28]=0;indices[33]=0;
    for(mode=0;mode<2;mode++)for(units=0;units<2;units++)for(trial=0;trial<500;trial++){
        u8 before[1792];u32 distance=0,total=0,scaled;u16 found=0;int race_a=0,race_b=0,track_a=0,track_b=0,mask_count=0,i;
        D_803BA800=mode?&owner_ptr:0;D_80146111=units;height=4+trial%17;
        for(i=0;i<1792;i++){rng=rng*1664525U+1013904223U;storage.bytes[i]=(u8)(rng>>24);}
        for(i=0;i<12;i++){
            u8 *record=storage.bytes+140+i*96;u32 d;u16 bits;
            rng=rng*1664525U+1013904223U;d=trial<4?(u32[]){0,1,0x7fffffffU,0xffffffffU}[trial]:rng;
            bits=trial<3?(u16[]){0,1,65535}[trial]:(u16)(rng>>4);U32_AT(record,88)=d;U16_AT(record,92)=bits;distance+=d;
            if(i<6){race_a+=pop(bits&255);race_b+=pop(bits&65280);expected_masks[mask_count++]=bits&255;expected_masks[mask_count++]=bits&65280;}
        }
        for(i=0;i<4;i++){
            u8 *record=storage.bytes+1292+i*64;u32 d;u16 bits;
            rng=rng*1664525U+1013904223U;d=trial<4?(u32[]){0,1,0x80000000U,0xffffffffU}[trial]:rng;
            bits=trial<3?(u16[]){0,1,65535}[trial]:(u16)(rng>>4);U32_AT(record,12)=d;U16_AT(record,60)=bits;total+=d;
            track_a+=pop(bits&255);track_b+=pop(bits&65280);expected_masks[mask_count++]=bits&255;expected_masks[mask_count++]=bits&65280;
        }
        for(i=0;i<8;i++){u16 n; rng=rng*1664525U+1013904223U;n=trial<3?(u16[]){0,1,65535}[trial]:(u16)rng;U16_AT(storage.bytes,1556+i*12)=n;found+=n;}
        scaled=units?(u32)(distance*10U)/6U:distance;
        sprintf(expected[0],"%01d.%01d",(int32_t)(scaled/10U),(int32_t)(scaled%10U));
        sprintf(expected[1],"%d",(int32_t)total);sprintf(expected[2],"%d",found);
        sprintf(expected[3],"%d/48",race_a);sprintf(expected[4],"%d/48",race_b);sprintf(expected[5],"%d/32",track_a);sprintf(expected[6],"%d/32",track_b);
        memcpy(before,storage.bytes,1792);current_case=total_cases;draws=counts=heights=formats=setups=fonts=dispatches=boxes=0;
        func_803ABEC8();
        check(draws==(mode?14:0)&&counts==(mode?20:0)&&heights==(mode?8:0)&&formats==(mode?7:0));
        check(setups==mode&&fonts==mode&&dispatches==mode&&boxes==mode);
        check(!memcmp(before,storage.bytes,1792)&&D_803BA800==(mode?&owner_ptr:0)&&D_80146111==units);total_cases++;
    }
    printf("{\"cases\":%d,\"submitted_complete_group\":true,\"draw_and_format_doubles\":true,\"input_bytes_unchanged\":true,\"unsigned_wraparound\":true}\n",total_cases);return 0;
}
