#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stddef.h>
typedef int8_t s8;typedef uint8_t u8;typedef int16_t s16;typedef int32_t s32;typedef uint32_t u32;typedef float f32;
typedef struct Slot18 {s16 f0;s8 f2,f3;u8 pad[20];} Slot18;
typedef struct AudioCfg {u8 pad0[12];s8 music,sfx,track,mono;u8 remaining[25];} AudioCfg;
typedef struct OSMesgQueue {int id;} OSMesgQueue;
#define M2C_FIELD(p,t,o) (*(t)((char *)(p)+(o)))
_Static_assert(sizeof(Slot18)==24,"slot stride");
_Static_assert(sizeof(AudioCfg)==41,"option extent");
_Static_assert(offsetof(AudioCfg,music)==12 && offsetof(AudioCfg,track)==14,"music fields");
static _Alignas(8) volatile Slot18 D_80142DD8[128];
static OSMesgQueue D_80142728={10},D_801427A8={11};
static AudioCfg D_80146108;
static s32 D_8010FFD4[19],D_8011735C;
static s8 D_8014978C;
static int events[16][4],event_count;
static void event(int kind,int queue,int flag,int slot){if(event_count==16)exit(4);events[event_count][0]=kind;events[event_count][1]=queue;events[event_count][2]=flag;events[event_count++][3]=slot;}
static s32 osRecvMesg(OSMesgQueue*q,void**message,s32 flag){if(message)exit(4);event(1,q->id,flag,-1);return 0;}
static s32 osSendMesg(OSMesgQueue*q,void*message,s32 flag){int slot=-1;if(message)slot=(Slot18*)message-(Slot18*)D_80142DD8;if(slot>=128)exit(4);event(2,q->id,flag,slot);return 0;}
#include "candidate.inc"
struct Result {u32 seed;u8 pool[128][24];int events[16][4],count;};
static struct Result actual(void){struct Result r;memset(&r,0,sizeof r);r.seed=(u32)D_8011735C;memcpy(r.pool,(const void*)D_80142DD8,sizeof r.pool);memcpy(r.events,events,sizeof events);r.count=event_count;return r;}
static void fixture(int track,int volume,int option,u32 seed,int busy){int i;memset((void*)D_80142DD8,0xa5,sizeof D_80142DD8);for(i=0;i<128;i++){D_80142DD8[i].f0=123;D_80142DD8[i].f3=i<busy;}memset(&D_80146108,0,sizeof D_80146108);D_80146108.music=volume;D_80146108.track=option;D_8014978C=track;D_8011735C=(s32)seed;for(i=0;i<19;i++)D_8010FFD4[i]=i%5==0?-1:i%12;memset(events,0,sizeof events);event_count=0;}
static void model_event(struct Result*r,int kind,int queue,int flag,int slot){int *e=r->events[r->count++];e[0]=kind;e[1]=queue;e[2]=flag;e[3]=slot;}
static void model_command(struct Result*r,int type,int song,float volume,float time){int slot=0;int16_t id=-1;while(r->pool[slot][3])slot++;memcpy(r->pool[slot],&id,2);r->pool[slot][2]=type;r->pool[slot][3]=1;if(type==10){memcpy(r->pool[slot]+4,&volume,4);memcpy(r->pool[slot]+8,&time,4);r->pool[slot][12]=1;r->pool[slot][13]=0;}else{int32_t value=song;memcpy(r->pool[slot]+4,&value,4);r->pool[slot][8]=1;}model_event(r,1,10,1,-1);model_event(r,2,10,0,-1);model_event(r,2,11,0,slot);}
/* Independent field/event model. Unsigned arithmetic specifies the native
 * 32-bit LCG wrap; candidate C runs with -fwrapv for its inherited signed seed. */
static struct Result model(int track,int volume,int option,int resume){struct Result r=actual();int selected;float v,time;if(!volume)return r;selected=track%5==0?-1:track%12;if(selected<0){unsigned draws=0;do{u32 raw;float scaled;r.seed=r.seed*UINT32_C(1103515245)+12345;raw=(r.seed>>16)&32767;scaled=(float)raw*12.0f;scaled=scaled/32768.0f;selected=(int)scaled;if(++draws>100)exit(4);}while(selected==5);}v=(float)volume/10.0f;if(v<0)v=0;if(v>1)v=1;time=resume?1.0f:0.05f;model_command(&r,10,0,v,time);if(option==12)model_command(&r,0,selected,0,0);else if(option<12)model_command(&r,0,option,0,0);return r;}
int main(void){static const int volumes[]={-128,-1,0,1,5,10,127};static const int options[]={-128,-1,0,5,11,12,13,127};static const int resumes[]={-1,0,1};static const int busy_values[]={0,1,126};u32 seeds[]={0,1,2,0x7fffffff,0x80000000,0xffffffff,0x12345678,0,0};unsigned track,v,o,r,s,b,cases=0;u32 n;
/* Deterministically include one and two successive rejected random draws. */
for(n=0;;n++){u32 x=n*UINT32_C(1103515245)+12345,y=x*UINT32_C(1103515245)+12345;unsigned a=((x>>16)&32767)*12/32768,z=((y>>16)&32767)*12/32768;if(a==5&&!seeds[7])seeds[7]=n;if(a==5&&z==5){seeds[8]=n;break;}}
for(track=0;track<19;track++)for(v=0;v<7;v++)for(o=0;o<8;o++)for(r=0;r<3;r++)for(s=0;s<9;s++)for(b=0;b<3;b++){struct Result expected,got;fixture(track,volumes[v],options[o],seeds[s],busy_values[b]);expected=model(track,volumes[v],options[o],resumes[r]);func_800D6160(resumes[r]);got=actual();if(memcmp(&expected,&got,sizeof got)){fputs("music differential mismatch\n",stderr);return 3;}cases++;}if(cases!=86184)return 4;puts("86184 finite music state-and-queue fixtures");return 0;}
