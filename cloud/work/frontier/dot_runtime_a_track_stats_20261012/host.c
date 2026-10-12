/* Executes the actual complete submitted group. Draw services and formatter
 * are deterministic host doubles, not retail helper/gameplay execution. */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>
#include <stdint.h>
#include "groups/runtime_a_stats_menu_20261006/group.c"
Language countdown_state;
Profile **D_803BA800;
s8 D_803BA858,D_80146111;
u8 D_80150F88[12*96];
FiveParts *D_803B6530;
f32 D_803BA90C;
u8 D_803B8AE8[]="%d",D_803B8AEC[]="%d",D_803B8AF0[]="%d",D_803B8AF4[]="%d";
u8 D_803B8AF8[]="%d",D_803B8AFC[]="%d",D_803B8B00[]="%d",D_803B8B04[]="%d",D_803B8B08[]="%d";
u8 D_803B8B0C[]="%01d.%01d",D_803B8B18[]="%d/8",D_803B8B20[]="%d/8";
u8 D_803B8BF4[]="--:--.---",D_803B8C00[]="%.2d:%.2d.%.3d";
static int current_case,profile,unit,height,draws,counts,heights,formats,font_calls;
static int expected_x[14],expected_y[14],expected_rows[14],row_count;
static u8 *record;static char labels[15][8];static char expected_values[14][48];
static void check(int ok){if(!ok){fprintf(stderr,"track panel case %d draw %d count %d failure\n",current_case,draws,counts);exit(1);}}
void func_800B669C(u32 a,u32 b){check(a==0&&b==3);}
s32 object_create(s32 font){check(font==11||font==2);font_calls++;return 0;}
void dispatch_handler(s32 mode){check(mode==1);}
s16 sound_pitch_diff_calc(void *text,s16 right){int i=expected_rows[draws/2];check(right==expected_x[i]-4);return right-(s16)strlen(text);}
s16 object_bytes_sum_global(void){heights++;return height;}
void crowd_cheer_play(FiveParts *box,s32 left,s32 top,s32 right,s32 bottom){check(box==D_803B6530&&left==12&&top==86&&right==308&&bottom==94+height*8);}
u8 *func_800BE6A4(u8 *out,u8 *in){check(in==D_803B8BF4);strcpy((char *)out,(char *)in);return out;}
void fcvt_wrapper(u8 *out,const u8 *fmt,...){va_list ap;int n;va_start(ap,fmt);n=vsprintf((char *)out,(const char *)fmt,ap);va_end(ap);check(n>=0&&n+1<=48);formats++;}
s32 func_800B78A4(u32 x,u8 n){s32 count=0;u32 expect=counts?U16_AT(record,92)&65280U:U16_AT(record,92)&255U;check(n==16&&x==expect);counts++;while(n--){count+=x&1;x>>=1;}return count;}
void state_utility(s16 x,s16 y,void *text){int i=expected_rows[draws/2];check(y==expected_y[i]);if(!(draws&1)){int label=i==11&&unit?14:i;check(x==expected_x[i]-4-(int)strlen(labels[label]));check(!strcmp(text,labels[label]));}else{check(x==expected_x[i]+4);check(!strcmp(text,expected_values[i]));}draws++;}
static void time_text(char *out,float time){if(time==0.0f)strcpy(out,"--:--.---");else sprintf(out,"%.2d:%.2d.%.3d",(u32)(time/60.0f)%100,(u32)time%60,(u32)(time*1000.0f)%1000);}
static int pop(unsigned v){int n=0;while(v){n+=v&1;v>>=1;}return n;}
int main(void){
    static union {u32 align;u8 bytes[140+12*96];} storage;
    static u16 indices[40];static u8 *indexed[15];
    Profile owner,*owner_ptr=&owner;u8 *stats=storage.bytes;int mode,units,selector,trial,k,total=0;u32 rng=7;
    owner.stats=&stats;countdown_state.indices=indices;countdown_state.indexed=indexed;
    for(k=0;k<15;k++){sprintf(labels[k],"row%d",k);indexed[k]=(u8 *)labels[k];}
    for(mode=0;mode<2;mode++)for(units=0;units<2;units++)for(selector=0;selector<12;selector++)for(trial=0;trial<100;trial++){
        static const float times[]={0.0f,1.0f,61.125f,3599.5f,65535.0f,3000000.0f};
        u8 before[96];u16 count;u32 distance;int track,x=0,y=0,i;
        profile=mode;unit=units;D_803BA800=mode?&owner_ptr:0;D_803BA858=selector;D_80146111=units;height=4+trial%17;
        track=selector/2+(selector&1?6:0);record=mode?storage.bytes+140+track*96:D_80150F88+track*96;
        memset(record,0,96);F32_AT(record,64)=times[trial%6];
        for(i=68;i<=84;i+=2){rng=rng*1664525U+1013904223U;U16_AT(record,i)=trial<3?(u16[]){0,1,65535}[trial]:(u16)rng;}
        rng=rng*1664525U+1013904223U;U32_AT(record,88)=trial<4?(u32[]){0,1,0x7fffffffU,0xffffffffU}[trial]:rng;U16_AT(record,92)=(u16)(trial*661);
        memcpy(before,record,96);count=U16_AT(record,68);distance=U32_AT(record,88);if(unit)distance=distance*10U/6U;
        time_text(expected_values[0],F32_AT(record,64));sprintf(expected_values[1],"%d",count);
        time_text(expected_values[2],count?F32_AT(record,64)/(float)(u32)count:0.0f);
        for(i=3;i<=10;i++)sprintf(expected_values[i],"%d",U16_AT(record,70+(i-3)*2));
        sprintf(expected_values[11],"%01d.%01d",distance/10U,distance%10U);
        sprintf(expected_values[12],"%d/8",pop(U16_AT(record,92)&255));sprintf(expected_values[13],"%d/8",pop(U16_AT(record,92)&65280));
        row_count=0;for(i=0;i<14;i++){
            if(i==0){x=100;y=90;}else if(i==3){x=110;y+=height;}else if(i==7){x=270;y=90+height*4;}
            else if(i==10){x=260;y=90;continue;}else if((i==12||i==13)&&!mode)continue;
            expected_x[i]=x;expected_y[i]=y;expected_rows[row_count++]=i;y+=height;
        }
        current_case=total;draws=counts=heights=formats=font_calls=0;D_803BA90C=-123.0f;
        func_803AD0D4();check(draws==row_count*2&&counts==(mode?2:0)&&heights==row_count+3&&font_calls==5);
        check(!memcmp(before,record,96)&&D_803BA800==(mode?&owner_ptr:0)&&D_803BA858==selector&&D_80146111==unit);
        total++;
    }
    printf("{\"cases\":%d,\"submitted_complete_group\":true,\"draw_and_format_doubles\":true,\"input_rows_unchanged\":true}\n",total);return 0;
}
