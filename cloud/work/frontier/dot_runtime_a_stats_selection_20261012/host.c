/* Finite actual-C equivalence harness. External rendering is an event log. */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>
#include "group.c"
#include "baseline.h"
Language countdown_state;
Slot D_803B5F28[18]; Camera D_80150B70[4];
f32 D_803B64E8=1.0f,D_803B64FC=300.0f;
Color4 D_801146BC={123},D_801146C0={456};
Profile **D_8012E6E0,**D_803BA800;
s32 D_803BA808,D_803BA830[4];
s8 D_803B65A4,D_803BA84D,D_803BA850,D_803BA854;
u8 D_803BA840[80]; u8 *D_8011EAD0[4];
TextBox D_803B8ACC={0,0,180,150}; FiveParts *D_803B6530;
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
void fcvt_wrapper(u8 *out,const u8 *fmt,...) {va_list ap;int n;event(11);text_event(fmt);va_start(ap,fmt);n=vsnprintf((char *)out,80,(const char *)fmt,ap);va_end(ap);if(n<0||n>=80)abort();text_event(out);}
void func_800ED66C(f32 f) {event(12);number(f);}
void func_800BEA3C(Color4 a,Color4 b) {event(13);event(a.word);event(b.word);}
void func_800F6928(f32 f) {event(14);number(f);}
void mode_byte_set(s32 n) {event(15);event(n);}
void camera_auto_follow(s16 a,s16 b,s16 c,s16 d,s16 e,s16 f,u8 *p) {event(16);event(a);event(b);event(c);event(d);event(e);event(f);text_event(p);}
s16 camera_zoom_fov(s16 w,u8 *p) {event(17);event(w);text_event(p);return 2;}
void music_fade(s16 x,s16 y,s16 w,void *p) {event(18);event(x);event(y);event(w);text_event(p);}
void crowd_cheer_play(FiveParts *p,s32 x,s32 y,s32 r,s32 b) {event(19);event(p!=0);event(x);event(y);event(r);event(b);}

int main(void) {
 static u8 *labels[200],*indexed[100];static u16 indices[60];
 static FileInfo files[2],*file_handles[2];static Profile nodes[12],*handles[12];
 static f32 angles[]={0.0f,0.6981316804885864f,1.0f,1.5707963705062866f,2.4434609413146973f,3.0f};
 static s32 states[]={-4,-3,-2,-1,0};
 unsigned trial,calls=0,old_used;int j,variant,owner;
 for(j=0;j<200;j++)labels[j]=(u8 *)"label";
 labels[143]=(u8 *)"Clear %s?";labels[146]=(u8 *)"Cleared %s";
 for(j=0;j<100;j++)indexed[j]=(u8 *)"choice";
 countdown_state.text=labels;countdown_state.indexed=indexed;countdown_state.indices=indices;
 for(j=0;j<2;j++) {files[j].controller=(u8)j;file_handles[j]=&files[j];D_8011EAD0[j]=(u8 *)"device";}
 for(j=0;j<12;j++) {handles[j]=&nodes[j];sprintf((char *)nodes[j].name,"Profile%02d",j);nodes[j].file=&file_handles[j%2];}
 for(j=0;j<12;j++) {nodes[j].next=j==11?0:&handles[j+1];nodes[j].previous=j==0?0:&handles[j-1];}
 D_8012E6E0=&handles[0];
 for(owner=0;owner<2;owner++)for(variant=0;variant<3;variant++)for(trial=0;trial<480;trial++) {
  D_803B64E8=(trial%3==0)?0.5f:1.0f;D_803B64FC=(trial%2)?280.0f:300.0f;
  D_803BA800=owner?&handles[trial%12]:0;D_803B65A4=(s8)(trial%2);D_803BA808=(s32)(trial%11);
  for(j=0;j<4;j++)D_803BA830[j]=states[trial%5];
  D_803BA84D=(s8)(variant==1);D_803BA854=(s8)(variant==2);D_803BA850=(s8)(trial%2);strcpy((char *)D_803BA840,"Name");
  for(j=0;j<18;j++) {Slot *s=&D_803B5F28[j];s->kind=(j%3==0)?11:(j%3==1)?9:2;s->alpha=(u8)((trial%5==0)?0:127+(trial%2)*128);s->angle=angles[(trial+j)%6];s->position[0]=(f32)j;s->position[1]=(f32)(j*2);s->position[2]=100.0f+(f32)j;}
  used=0;baseline_803AA3D8();old_used=used;memcpy(prior,events,used*sizeof(u32));used=0;func_803AA3D8();
  if(used!=old_used||memcmp(prior,events,used*sizeof(u32))) {fprintf(stderr,"event mismatch owner=%d variant=%d trial=%u\n",owner,variant,trial);return 1;}
  calls++;
 }
 printf("%u actual-C renderer cases; exact stub-event equivalence\n",calls);return 0;
}
