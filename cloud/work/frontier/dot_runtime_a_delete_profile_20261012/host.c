/* Finite actual-C equivalence harness. External rendering is an event log. */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>
#include "group.c"
#include "baseline.h"
Language countdown_state;
Slot D_803B470C[4][22]; Camera D_80150B70[4]; Layout D_803B5D50[4][4];
f32 D_803B5D10=1.0f,D_803B5D24=300.0f;
s16 D_8014A108; Player D_8014A118[4]; Profile *D_80146150[4];
s32 D_801146AC[4]; Color4 D_801146BC={123},D_801146C0={456};
Profile **D_8012E6E0,**D_8012E6E4,**D_803BAC80[4];
void *D_803BAC68[4]; FiveParts *D_803BAC08[4];
s32 D_803BACE0[4],D_803BAD80[4];
s8 D_803BADB8[4],D_803BADBC[4],D_803BADC0[4],D_803BADC8[4];
s8 D_803BADCC[4],D_803BADD0[4],D_803BADD4[4],D_803BAD00[4][14];
u8 D_803BAC98[4][13],D_803BADD8[4][13];s16 D_803BACD0[4];
u8 *D_80117420,*D_8011EAD0[4];
u8 D_803B5EDC[8],D_803B5EE0[8],D_803B5EE4[8];
u8 D_803B899C[16],D_803B89A4[]="%s %s %s",D_803B89AC[16],D_803B89B4[16],D_803B89C0[16],D_803B89C8[16];
static u32 events[8192], prior[8192];static unsigned used;
static void event(u32 n) {if(used>=8192)abort();events[used++]=n;}
static void number(f32 f) {u32 n;memcpy(&n,&f,4);event(n);}
static void text_event(const u8 *p) {unsigned n=0;while(p[n]) {if(n>127)abort();event(p[n++]);}event(0);}
void render_helper(f32 f) {event(1);number(f);}
s32 object_create(s32 n) {event(2);event(n);return 19;}
void dispatch_handler(s32 n) {event(3);event(n);}
void func_800B669C(u32 a,u32 b) {event(4);event(a);event(b);}
void brake_light_update(s32 player,f32 *p,Camera *c,void *unused,s16 *out) {
 event(5);event(player);number(p[0]);number(p[1]);number(p[2]);
 event((u32)(c-D_80150B70));event(unused!=0);
 out[0]=(s16)(120+player*7+(s32)p[0]);out[1]=(s16)(25+(s32)p[1]);
}
s16 sound_pitch_diff_halved(void *p,s16 x) {event(6);event(x);text_event(p);return x-(s16)strlen(p)*3;}
s16 sound_pitch_diff_calc(void *p,s16 x) {event(7);event(x);text_event(p);return x-(s16)strlen(p)*6;}
s16 object_bytes_sum_global(void) {event(8);return 9;}
s32 object_manager_update(u8 *p,s16 n) {event(9);event(n);text_event(p);return (s32)strlen((char *)p)*8;}
void state_utility(s16 x,s16 y,void *p) {event(10);event(x);event(y);text_event(p);}
void fcvt_wrapper(u8 *out,const u8 *fmt,...) {va_list ap;int n;event(11);text_event(fmt);va_start(ap,fmt);n=vsnprintf((char *)out,128,(const char *)fmt,ap);va_end(ap);if(n<0||n>=128)abort();text_event(out);}
void func_800ED66C(f32 f) {event(12);number(f);}
void func_800BEA3C(Color4 a,Color4 b) {event(13);event(a.word);event(b.word);}
void func_800F6928(f32 f) {event(14);number(f);}
void mode_byte_set(s32 n) {event(15);event(n);}
void camera_auto_follow(s16 a,s16 b,s16 c,s16 d,s16 e,s16 f,u8 *p) {event(16);event(a);event(b);event(c);event(d);event(e);event(f);text_event(p);}
s16 camera_zoom_fov(s16 w,u8 *p) {event(17);event(w);text_event(p);return 2;}
void music_fade(s16 x,s16 y,s16 w,void *p) {event(18);event(x);event(y);event(w);text_event(p);}
void crowd_cheer_play(FiveParts *p,s32 x,s32 y,s32 r,s32 b) {event(19);event(p!=0);event(x);event(y);event(r);event(b);}

int main(void) {
 static u8 *labels[200],*indexed[80];static u16 indices[30];
 static FileInfo files[2],*file_handles[2];static Profile *nodes[12];
 static f32 angles[]={0.0f,0.6981316804885864f,1.0f,1.5707963705062866f,2.4434609413146973f,3.0f};
 static s8 states[]={-5,-4,-3,-2,-1,0,8,12};
 unsigned trial,calls=0,old_used;int j,k,player,count,variant;
 for(j=0;j<200;j++)labels[j]=(u8 *)"label";
 labels[145]=(u8 *)"Deleted %s";for(j=0;j<80;j++)indexed[j]=(u8 *)"choice";
 countdown_state.text=labels;countdown_state.indexed=indexed;countdown_state.indices=indices;
 for(j=0;j<2;j++) {files[j].controller=(u8)j;file_handles[j]=&files[j];D_8011EAD0[j]=(u8 *)"device";}
 for(j=0;j<12;j++) {nodes[j]=calloc(1,sizeof(Profile)+16);if(!nodes[j])abort();sprintf((char *)nodes[j]->name,"Profile%02d",j);nodes[j]->file=&file_handles[j%2];}
 for(j=0;j<12;j++) {nodes[j]->next=j==11?0:&nodes[j+1];nodes[j]->previous=j==0?0:&nodes[j-1];}
 D_8012E6E0=&nodes[0];D_8012E6E4=&nodes[11];
 for(count=1;count<=4;count++)for(player=0;player<count;player++)for(variant=0;variant<3;variant++)for(trial=0;trial<96;trial++) {
  D_8014A108=(s16)count;D_803B5D10=(trial%3==0)?0.5f:1.0f;D_803B5D24=(trial%2)?280.0f:300.0f;
  for(j=0;j<4;j++) {D_8014A118[j].owner=&nodes[(trial+j)%12];D_803BAC80[j]=(trial%4)?&nodes[(trial+j)%12]:0;
   D_803BADBC[j]=(s8)(trial%2);D_803BADC0[j]=states[trial%8];D_803BADC8[j]=(s8)(trial%14);D_803BADCC[j]=(s8)(variant==1);D_803BADD4[j]=(s8)(variant==2);D_803BADD0[j]=(s8)(trial%3);strcpy((char *)D_803BADD8[j],"Name");D_801146AC[j]=j+2;
   for(k=0;k<14;k++)D_803BAD00[j][k]=(s8)((trial+k)%2);
   for(k=0;k<22;k++) {Slot *s=&D_803B470C[j][k];s->kind=(k%3==0)?9:(k%3==1)?7:2;s->alpha=(u8)((trial%5==0)?0:127+(trial%2)*128);s->angle=angles[(trial+k)%6];s->position[0]=(f32)k;s->position[1]=(f32)(k*2);s->position[2]=100.0f+(f32)k;}
   for(k=0;k<4;k++) {D_803B5D50[k][j].x=100+j*12;D_803B5D50[k][j].y=30;D_803B5D50[k][j].width=120;D_803B5D50[k][j].height=180;}
  }
  used=0;baseline_803A9088(player);old_used=used;memcpy(prior,events,used*sizeof(u32));used=0;func_803A9088(player);
  if(used!=old_used||memcmp(prior,events,used*sizeof(u32))) {fprintf(stderr,"event mismatch count=%d player=%d variant=%d trial=%u\n",count,player,variant,trial);return 1;}
  calls++;
 }
 for(j=0;j<12;j++)free(nodes[j]);
 printf("%u actual-C renderer cases; exact stub-event equivalence\n",calls);return 0;
}
