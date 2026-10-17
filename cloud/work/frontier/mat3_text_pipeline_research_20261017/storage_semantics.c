#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "storage_under_test.c"
#include "length_under_test.c"
TextColor D_80153F60; NativeTextColor D_80149B48;
f32 D_80153F80,D_80114744,D_80114748;
s16 D_80153FD0,D_80154180,D_80149D92,D_80149D9E;
s8 D_80154184,D_80154194,D_8015419C,D_8011473C,D_8011474C,D_80149DA0,D_80149B60,D_80149B70;
s32 D_80149DD0; u8 D_80149DD8[800];
static char events[16];static unsigned ne,draw_len;static const void *draw_text;
static void event(char e){if(ne>=sizeof events)abort();events[ne++]=e;}
void du_gfx_lock(void){event('L');}void du_gfx_unlock(void){event('U');}
void audio_doppler_calc(const u8 *p,u16 n){event('R');draw_text=p;draw_len=n;}
char *func_800A473C(char *d,const char *s){event('S');return strcpy(d,s);}
char *func_80092DCC(char *d,const char *s,u32 n){event('N');return strncpy(d,s,n);}
void *root_memcpy(void *d,const void *s,u32 n){return memcpy(d,s,n);}
#include "defaults_under_test.c"

typedef struct {u8 buf[800],color[4];int used,inited;float trans;short x,y;signed char font,lead,space;char ev[16];unsigned n,draw;} Model;
static void emit(Model*m,unsigned v){if(m->used<0||m->used>=800)abort();m->buf[m->used++]=(u8)v;}
static void model(Model*m,const u8*text,short bound,unsigned chars,unsigned wide){
 unsigned units=chars+1;if(bound>=0&&units>(unsigned)bound+1)units=(unsigned)bound+1;
 unsigned len=wide?1+units*2:units;
 if(D_80114744>=0){m->ev[m->n++]='R';m->draw=len;return;}
 if(D_8011473C)m->ev[m->n++]=bound<0?'S':'N';m->ev[m->n++]='L';
 if(!m->inited){m->inited=1;m->used=0;m->color[0]=m->color[1]=m->color[2]=230;m->color[3]=255;m->trans=-1;m->x=m->y=0;m->font=m->lead=m->space=-1;}
 int changed[6]={m->font!=D_80149DA0,m->lead!=D_80149B60,m->space!=D_80149B70,m->x!=D_80149D92||m->y!=D_80149D9E,m->trans!=D_80114748,memcmp(m->color,D_80149B48.fore.rgba,4)!=0};
 int need=(int)len+2;for(int i=0;i<6;i++)if(changed[i])need+=i<3?2:5;
 if(m->used+need<800){
  for(unsigned i=0;i<6;i++)if(changed[i]){
   emit(m,i);
   switch(i){
    case 0:m->font=D_80149DA0;emit(m,(u8)m->font);break;
    case 1:m->lead=D_80149B60;emit(m,(u8)m->lead);break;
    case 2:m->space=D_80149B70;emit(m,(u8)m->space);break;
    case 3:m->x=D_80149D92;m->y=D_80149D9E;emit(m,(uint16_t)m->x>>8);emit(m,(u8)m->x);emit(m,(uint16_t)m->y>>8);emit(m,(u8)m->y);break;
    case 4:{uint32_t bits;m->trans=D_80114748;memcpy(&bits,&m->trans,4);for(int shift=24;shift>=0;shift-=8)emit(m,bits>>shift);break;}
    case 5:memcpy(m->color,D_80149B48.fore.rgba,4);for(int j=0;j<4;j++)emit(m,m->color[j]);break;
   }
  }
  emit(m,6);emit(m,len);for(unsigned j=0;j<len;j++)emit(m,text[j]);
 }
 m->ev[m->n++]='U';
}
int main(void){
 static const short bounds[]={-32768,-1,0,1,2,32,127,255};
 static const unsigned lengths[]={0,1,2,31,63};
 static const int used[]={0,1,700,774,775,776,777,778,797,798,799,800};
 uint64_t count=0;u8 text[260];
 for(unsigned mask=0;mask<64;mask++)for(unsigned init=0;init<2;init++)for(unsigned depth=0;depth<3;depth++)for(unsigned finished=0;finished<2;finished++)for(unsigned wide=0;wide<2;wide++)for(unsigned li=0;li<5;li++)for(unsigned bi=0;bi<8;bi++)for(unsigned ui=0;ui<12;ui++){
  memset(text,0,sizeof text);unsigned chars=lengths[li];
  if(wide){text[0]=255;for(unsigned j=0;j<chars;j++){text[1+2*j]=(u8)(j%127+1);text[2+2*j]=(u8)(128+j);}}
  else for(unsigned j=0;j<chars;j++)text[j]=(u8)(j%200+1);
  D_80114744=(float)depth-1;D_8011473C=(s8)finished;D_8011474C=(s8)init;D_80149DD0=used[ui];
  D_80149DA0=127;D_80149B60=-128;D_80149B70=127;D_80149D92=-32768;D_80149D9E=32767;D_80114748=.5f;
  D_80154184=(mask&1)?-1:D_80149DA0;D_80154194=(mask&2)?127:D_80149B60;D_8015419C=(mask&4)?-128:D_80149B70;
  D_80153FD0=(mask&8)?32767:D_80149D92;D_80154180=(mask&8)?-32768:D_80149D9E;D_80153F80=(mask&16)?-1:D_80114748;
  for(unsigned j=0;j<8;j++)D_80149B48.fore.rgba[j]=(u8)(31+j*32);
  for(unsigned j=0;j<4;j++)D_80153F60.fore.rgba[j]=(u8)(D_80149B48.fore.rgba[j]^((mask&32)?255:0));
  memset(D_80149DD8,0xa5,800);ne=draw_len=0;draw_text=0;memset(events,0,sizeof events);
  Model m={0};memcpy(m.buf,D_80149DD8,800);memcpy(m.color,D_80153F60.fore.rgba,4);m.used=D_80149DD0;m.inited=init;m.trans=D_80153F80;m.x=D_80153FD0;m.y=D_80154180;m.font=D_80154184;m.lead=D_80154194;m.space=D_8015419C;
  model(&m,text,bounds[bi],chars,wide);menu_input_process(text,bounds[bi]);count++;
  if(memcmp(m.buf,D_80149DD8,800)||memcmp(m.color,D_80153F60.fore.rgba,4)||m.used!=D_80149DD0||m.inited!=D_8011474C||m.trans!=D_80153F80||m.x!=D_80153FD0||m.y!=D_80154180||m.font!=D_80154184||m.lead!=D_80154194||m.space!=D_8015419C||m.n!=ne||memcmp(m.ev,events,ne)||m.draw!=draw_len||(depth&&draw_text!=text)){
   fprintf(stderr,"storage differential failure at fixture %llu\n",(unsigned long long)count);return 3;
  }
  for(unsigned j=0;j<8;j++)if(D_80149B48.fore.rgba[j]!=(u8)(31+j*32)){fputs("current color mutated\n",stderr);return 4;}
 }
 printf("storage semantics passed: %llu fixtures\n",(unsigned long long)count);return 0;
}
