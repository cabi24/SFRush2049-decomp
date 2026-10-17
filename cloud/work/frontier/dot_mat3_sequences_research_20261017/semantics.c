#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
typedef int32_t s32;typedef uint32_t u32;typedef int8_t s8;typedef uint8_t u8;typedef float f32;
typedef struct Link {struct Link *next,*prev;} Link;
typedef struct List {u8 indirect,doubly,pad[2];u32 count;Link *head,*tail;} List;
typedef struct Sequence { Link ll; u32 id; s8 free,active,stop; f32 startTime,timeout; u8 next; s32 ids[4]; s32 subId,replace; u8 priority; s32 sndTag; } Sequence;
typedef struct FxTag { Link ll; List *list; u32 id; s32 state,type; s8 paused,loop; u8 numCmds,priority; s32 wpriority; f32 startTime,vol,lrpan,fbpan,pitch; s32 fxid,subId; u32 sysId; void *data; } FxTag;
typedef FxTag Effect;
static List D_80149860;static float now;static Sequence nodes[3];static FxTag tags[9];
static int events[256][5],ne,played,freed,stopped,freedtag;
static void ev(int k,int a,int b,int c,int d){if(ne>=256)exit(4);events[ne][0]=k;events[ne][1]=a;events[ne][2]=b;events[ne][3]=c;events[ne][4]=d;ne++;}
static s32 func_80095198(f32 x,f32 dt){f32 sum=x+dt;if(now<x)sum-=14400.0f;return sum<now;}
static FxTag *func_80091BA8(s32 id){ev(1,id,0,0,0);return id>=0&&id<9?tags+id:0;}
static void scheduler_recv(s32 id){stopped++;ev(2,id,0,0,0);}
static s32 entity_state_check(u32 id){ev(3,id,0,0,0);return id&1;}
static void audio_effect_setup(Effect *t){freedtag++;ev(4,t-tags,0,0,0);t->state=0;}
static void entity_flag_check(s32 *p){Sequence *s=(Sequence *)p;freed++;ev(5,s-nodes,0,0,0);s->free=1;s->active=0;s->ll.next=0;}
static s32 entity_flags_apply(s32 fx,s32 sub,s32 replace,u8 priority){int id=6+played++;ev(6,fx,sub,replace,priority);if(id>=9)exit(4);tags[id].state=1;return id;}

#include "candidate_body.inc"
#include "donor_body.inc"

struct Result {u32 id[3];int free[3],active[3],next[3],tag[3],effect[9],data[9];int ne,events[256][5],played,freed,stopped,freedtag;};
static void fixture(unsigned n){static const float times[]={0,1,500,14399,14400};static const float deltas[]={-1,0,0.25f,20,15000};int i,k;memset(nodes,0,sizeof nodes);memset(tags,0,sizeof tags);memset(events,0,sizeof events);ne=played=freed=stopped=freedtag=0;now=times[n%5];
 for(i=0;i<3;i++){Sequence *s=nodes+i;s->ll.next=i==2?0:&nodes[i+1].ll;s->active=1;s->id=0xaabbcc00+i;s->stop=(n>>(i+3))&1;s->startTime=times[(n+i)%5];s->timeout=deltas[(n/5+i)%5];s->next=(n>>(i*2))&3;s->sndTag=(n/11+i)%8-2;s->subId=i-1;s->replace=(n+i)%3;s->priority=n+i;for(k=0;k<4;k++)s->ids[k]=(n>>(k+i))&1?-1:k+10*i;}
 for(i=0;i<9;i++){tags[i].state=(n/7+i)%6-1;tags[i].numCmds=(n>>(i%5))&1;tags[i].sysId=(n+i)&1;}
 D_80149860.head=&nodes[0].ll;}
static struct Result result(void){struct Result r;int i;memset(&r,0,sizeof r);for(i=0;i<3;i++){r.id[i]=nodes[i].id;r.free[i]=nodes[i].free;r.active[i]=nodes[i].active;r.next[i]=nodes[i].next;r.tag[i]=nodes[i].sndTag;}
 for(i=0;i<9;i++){r.effect[i]=tags[i].state;r.data[i]=tags[i].data?(Sequence *)tags[i].data-nodes+1:0;}r.ne=ne;memcpy(r.events,events,sizeof events);r.played=played;r.freed=freed;r.stopped=stopped;r.freedtag=freedtag;return r;}
int main(void){unsigned n;struct Result a,b;for(n=0;n<32768;n++){fixture(n);func_800988D8();a=result();fixture(n);donor_root();b=result();if(memcmp(&a,&b,sizeof a)){fputs("sequence differential mismatch\n",stderr);return 3;}}puts("32768 finite sequence-list fixtures");return 0;}
