#include <stdint.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
typedef uint8_t u8;typedef int32_t s32;typedef uintptr_t u32;
typedef struct OSMesgQueue {int unused;} OSMesgQueue;
typedef struct PakNode PakNode;
typedef struct PakRequest {PakNode *next;uintptr_t opaque4;void (*done)(PakNode *);uintptr_t opaque12;u8 port,file;u8 name[35],extension[11];uintptr_t file_size,pages;u8 **buffer;} PakRequest;
struct PakNode {PakRequest *request;};
typedef struct PakList {u8 indirect,doubly,opaque2[2];unsigned count;PakNode *head,*tail;} PakList;
static PakNode nodes[4];static PakRequest requests[4];static PakList D_80144D60[4],D_801460E0;static OSMesgQueue D_80152770;
static u8 storage[4],*buffers[4];static unsigned modes[4],fixture_id;static int events[128][4],ne;
static int nodeid(PakNode *n){return n?(int)(n-nodes):-1;}
static int listid(PakList *l){return l==&D_801460E0?4:(int)(l-D_80144D60);}
static int bufferid(u8 **b){return b?(int)(b-buffers):-1;}
static void event(int a,int b,int c,int d){if(ne==128)exit(4);events[ne][0]=a;events[ne][1]=b;events[ne][2]=c;events[ne++][3]=d;}
/* Controlled list-service effects; they are shared service models, not a
 * claim to execute the native linked-list implementation. */
static void unlink_model(PakList *l,PakNode *n){PakNode *p=0,*x=l->head;int count=0;while(x&&x!=n&&count++<4){p=x;x=x->request->next;}if(!x)return;if(p)p->request->next=x->request->next;else l->head=x->request->next;if(l->tail==x)l->tail=p;if(l->count)l->count--;}
static void append_model(PakList *l,PakNode *n){if(l->tail)l->tail->request->next=n;else l->head=n;l->tail=n;n->request->next=0;l->count++;}
static void callback(PakNode *n){int i=nodeid(n);event(1,i,modes[i],0);if(modes[i]==1)n->request->next=0;else if(modes[i]==2)n->request->buffer=(fixture_id&1)?0:&buffers[(i+1)&3];else if(modes[i]==3){unsigned p=n->request->port;unlink_model(&D_80144D60[p],n);n->request->port=(p+1)&3;append_model(&D_80144D60[(p+1)&3],n);}}
static s32 osRecvMesg(OSMesgQueue *q,void **message,s32 flag){event(2,q==&D_80152770,message==0,flag);return 0;}
static void audio_reverb_update(u32 address,s32 tag){event(3,bufferid((u8 **)address),tag,0);}
static s32 osSendMesg(OSMesgQueue *q,void *message,s32 flag){event(4,q==&D_80152770,message==0,flag);return 0;}
static void func_8009211C(PakList *l,PakNode *n){event(5,listid(l),nodeid(n),0);unlink_model(l,n);}
static void func_80091FBC(PakList *l,PakNode *n,PakNode *before){event(6,listid(l),nodeid(n),nodeid(before));if(before){n->request->next=before;l->head=n;l->count++;}else append_model(l,n);}
#include "candidate_body.inc"
/* Independent index-driven cleanup model. Save the original successor before
 * modeled callbacks, then use the post-callback allocation and port fields. */
static void model(s32 port){int current=nodeid(D_80144D60[port].head),iterations=0;while(current>=0){int successor=nodeid(requests[current].next);PakNode *n=&nodes[current];PakRequest *r=&requests[current];if(++iterations>4)exit(4);if(r->done)callback(n);if(r->buffer){int id=bufferid(r->buffer);event(2,1,1,1);event(3,id,0,0);event(4,1,1,0);r->buffer=0;}event(5,r->port,current,0);unlink_model(&D_80144D60[r->port],n);event(6,4,current,nodeid(D_801460E0.head));n->request->next=D_801460E0.head;D_801460E0.head=n;if(!D_801460E0.tail)D_801460E0.tail=n;D_801460E0.count++;current=successor;}}
struct Result {int buffer[4],port[4],next[4],head[5],tail[5],count[5],events[128][4],ne;};
static void fixture(unsigned n){int i,length=(n>>2)%5;unsigned port=n&3;fixture_id=n;memset(requests,0,sizeof requests);memset(D_80144D60,0,sizeof D_80144D60);memset(&D_801460E0,0,sizeof D_801460E0);memset(events,0,sizeof events);ne=0;for(i=0;i<4;i++){nodes[i].request=&requests[i];buffers[i]=&storage[i];requests[i].port=port;requests[i].buffer=(n>>(12+i))&1?&buffers[i]:0;modes[i]=(n>>(4+2*i))&3;requests[i].done=modes[i]?callback:0;requests[i].next=i+1<length?&nodes[i+1]:0;}if(length){D_80144D60[port].head=&nodes[0];D_80144D60[port].tail=&nodes[length-1];D_80144D60[port].count=length;}}
static struct Result result(void){struct Result r;int i;memset(&r,0,sizeof r);for(i=0;i<4;i++){r.buffer[i]=bufferid(requests[i].buffer);r.port[i]=requests[i].port;r.next[i]=nodeid(requests[i].next);}for(i=0;i<5;i++){PakList*l=i==4?&D_801460E0:&D_80144D60[i];r.head[i]=nodeid(l->head);r.tail[i]=nodeid(l->tail);r.count[i]=l->count;}r.ne=ne;memcpy(r.events,events,sizeof events);return r;}
int main(void){unsigned n;struct Result a,b;for(n=0;n<65536;n++){fixture(n);func_800A3640(n&3);a=result();fixture(n);model(n&3);b=result();if(memcmp(&a,&b,sizeof a)){fputs("pak cleanup differential mismatch\n",stderr);return 3;}}puts("65536 finite cleanup state-and-service-trace fixtures");return 0;}
