/* Bounded host-C differential checks. Host pointers are not an O32 ABI proof. */
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>

typedef signed char s8; typedef unsigned char u8;
typedef unsigned short u16; typedef int s32; typedef unsigned int u32;
typedef float f32;
typedef struct Entry { s8 active,started,stopRequested,stopSent,flag4,blocked; u8 reserved6[2]; s32 id; u16 bank,sound; u32 voice,word20; } Entry;
typedef struct Node { struct Node *prev,*next; u8 kind,retries,reservedA[2]; Entry *entry; f32 value,stream,priority,route; } Node;
typedef struct List {u32 reserved[2];Node *first,*last;} List;
Entry *D_80144C48;
s32 D_80144DB8,D_801460F4,D_801460C8[5];
s8 D_80110284,D_8011028C;
List D_80146138,D_80146160;
static Entry entries[2][12]; static Node nodes[16];
static uint64_t trace_hash; static u32 seed,scenario,call_count; static int mutation;
void baseline_camera_transform(void); void candidate_camera_transform(void);
static void put(u32 x) { trace_hash^=x; trace_hash*=UINT64_C(1099511628211); }
static int node_id(Node *n) { return n ? (int)(n-nodes)+1 : 0; }
static int entry_id(Entry *p) { int a,b;if(!p)return 0;for(a=0;a<2;a++)for(b=0;b<12;b++)if(p==&entries[a][b])return 1+a*12+b;abort(); }
static u32 rng(void) { seed^=seed<<13;seed^=seed>>17;seed^=seed<<5;return seed; }
static u32 event(u32 tag,u32 a,u32 b,u32 c) {
 u32 r;put(tag);put(a);put(b);put(c);call_count++;
 /* Deterministic permitted callback mutations probe required post-call reloads. */
 if(mutation && call_count%5==0) D_80144C48=(D_80144C48==entries[0]?entries[1]:entries[0]);
 if(mutation && call_count%7==0) nodes[(scenario+call_count)%16].entry=&entries[(scenario>>1)&1][(scenario+call_count)%12];
 r=(scenario*17u+call_count*13u+a*3u+tag)%5u;return r;
}
void audio_timing_sync(s32 id) { (void)event(1,(u32)id,0,0); }
s32 func_8001FE58(u32 voice) { return (s32)(event(2,voice,0,0)&1); }
s32 func_8001FEA4(u32 voice,u8 value) { return (s32)(event(3,voice,value,0)&1); }
s32 func_8001FF4C(u32 voice,u8 value) { return (s32)(event(4,voice,value,0)&1); }
s32 func_8001FFA0(u32 voice,u16 value) { return (s32)(event(5,voice,value,0)&1); }
s32 func_8001FFF4(u32 voice,u8 value) { return (s32)(event(6,voice,value,0)&1); }
s32 func_80020174(u16 sound,u8 a,u8 b) { u32 r=event(7,sound,a,b);return r==0?-1:(s32)(sound+100); }
u32 func_800201D0(u32 voice) { return event(8,voice,0,0)<2?(u32)-1:voice; }
void func_80020518(u8 value) { (void)event(9,value,0,0); }
/* Direct doubly-linked specializations of accepted list helper contracts.
   Node.prev is the underlying next link; Node.next is the underlying prev. */
void func_8009211C(List *l,Node *n) {
 put(10);put(l==&D_80146160?1:2);put((u32)node_id(n));
 if(!n)return;
 if(n->prev)n->prev->next=n->next;else l->last=n->next;
 if(n->next)n->next->prev=n->prev;else l->first=n->prev;
 l->reserved[1]--;
}
void func_80091FBC(List *l,Node *n,Node *before) {
 put(11);put(l==&D_80146160?1:2);put((u32)node_id(n));put((u32)node_id(before));
 if(!n)return;
 if(before){if(before->next)before->next->prev=n;else l->first=n;n->next=before->next;before->next=n;}
 else{n->next=l->last;if(l->last)l->last->prev=n;else l->first=n;l->last=n;}
 n->prev=before;l->reserved[1]++;
}
static void initialize(u32 index) {
 int a,b,n;static const f32 volumes[]={0.0f,0.25f,0.5f,1.0f};static const f32 signed_values[]={-1.0f,-0.5f,0.0f,1.0f};
 memset(entries,0,sizeof(entries));memset(nodes,0,sizeof(nodes));memset(&D_80146138,0,sizeof(List));memset(&D_80146160,0,sizeof(List));
 scenario=index;seed=index+0x097ca0u;call_count=0;trace_hash=UINT64_C(1469598103934665603);mutation=(index%3)==0;
 D_80144C48=entries[0];D_801460F4=(s32)(index%13);D_80144DB8=(s32)(index%9)-2;
 D_80110284=index%17?1:0;D_8011028C=(s8)((index/2)%2);
 for(a=0;a<5;a++)D_801460C8[a]=(s32)(rng()%6);
 for(a=0;a<2;a++)for(b=0;b<12;b++){
  Entry *e=&entries[a][b];e->active=(s8)(rng()%2);e->started=(s8)(rng()%2);e->stopRequested=(s8)(rng()%2);e->stopSent=(s8)(rng()%2);e->flag4=(s8)(rng()%2);e->blocked=(s8)(rng()%2);e->id=100+a*12+b;e->sound=(u16)(30+b);e->voice=rng()%4?50+b:(u32)-1;e->bank=(u16)a;e->word20=rng();
 }
 n=(int)(index%17);
 for(b=0;b<16;b++){
  Node *p=&nodes[b];p->kind=(u8)((index+b)%6);p->retries=(u8)((index+b)%12);
  p->entry=&entries[rng()%2][rng()%12];
  if((p->kind==1||p->kind==4||p->kind==5)&&rng()%3==0)p->entry=0;
  p->value=volumes[rng()%4];p->stream=signed_values[rng()%4];p->priority=signed_values[rng()%4];p->route=volumes[rng()%4]*2.0f;
  if(p->kind==3){if(rng()%3==0)p->value=-2.0f;if(rng()%3==0)p->stream=-2.0f;if(rng()%3==0)p->priority=-2.0f;if(rng()%3==0)p->route=-2.0f;}
  if(p->kind==4)p->value=(f32)(index%256);
  if(b<n){p->next=b?&nodes[b-1]:0;p->prev=b+1<n?&nodes[b+1]:0;}
 }
 D_80146160.first=n?&nodes[0]:0;D_80146160.last=n?&nodes[n-1]:0;D_80146160.reserved[1]=(u32)n;
}
static uint64_t capture(void) {
 int a,b;u32 bits;put((u32)entry_id(D_80144C48));put((u32)D_80144DB8);put((u32)D_801460F4);put((u32)D_80110284);put((u32)D_8011028C);
 for(a=0;a<5;a++)put((u32)D_801460C8[a]);
 for(a=0;a<2;a++)for(b=0;b<12;b++){Entry *e=&entries[a][b];put((u32)e->active);put((u32)e->started);put((u32)e->stopRequested);put((u32)e->stopSent);put((u32)e->flag4);put((u32)e->blocked);put((u32)e->id);put(e->voice);put(e->bank);put(e->sound);put(e->word20);}
 for(b=0;b<16;b++){Node *p=&nodes[b];put((u32)node_id(p->prev));put((u32)node_id(p->next));put(p->kind);put(p->retries);put((u32)entry_id(p->entry));memcpy(&bits,&p->value,4);put(bits);memcpy(&bits,&p->stream,4);put(bits);memcpy(&bits,&p->priority,4);put(bits);memcpy(&bits,&p->route,4);put(bits);}
 put((u32)node_id(D_80146138.first));put((u32)node_id(D_80146138.last));put(D_80146138.reserved[1]);put((u32)node_id(D_80146160.first));put((u32)node_id(D_80146160.last));put(D_80146160.reserved[1]);return trace_hash;
}
int main(void) {
 u32 i;uint64_t want,got;
 for(i=0;i<2048;i++){
  initialize(i);baseline_camera_transform();want=capture();
  initialize(i);candidate_camera_transform();got=capture();
  if(want!=got){printf("{\"status\":\"mismatch\",\"case\":%u,\"expected\":\"%016llx\",\"actual\":\"%016llx\"}\n",i,(unsigned long long)want,(unsigned long long)got);return 1;}
 }
 puts("{\"status\":\"bounded_pass\",\"cases\":2048}");return 0;
}
