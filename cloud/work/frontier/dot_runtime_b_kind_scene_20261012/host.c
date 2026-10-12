/* Actual submitted source versus current baseline. Helpers are deterministic
 * synchronous doubles; this is logical host behavior, not retail execution. */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "group.c"
#include "baseline.inc"
s16 D_80151AD0;
Player D_80152818[4];
Status D_80395C00[4];
Camera D_80150B70[4];
Scene D_8012E700[16];
s32 D_80393B74[4];
u16 D_801427C0[256];
Point D_803941D0[4][4];
Color D_803942A4;
f32 D_80116178;
typedef struct Event { s32 op,a,b,c,d,e; } Event;
static Event events[256];
static s32 nevent, mode, selected_player, seed, token;
static void fail(const char *s) { fprintf(stderr,"host failure: %s seed=%d mode=%d\n",s,seed,mode); exit(1); }
static s32 bits(f32 f) { s32 b;memcpy(&b,&f,4);return b; }
static void record(s32 op,s32 a,s32 b,s32 c,s32 d,s32 e) {
    Event *v;
    if(nevent>=256)fail("event overflow");v=&events[nevent++];v->op=op;v->a=a;v->b=b;v->c=c;v->d=d;v->e=e;
    if(mode==op) {
        D_80152818[selected_player].kind=(seed+op)%9;
        D_80395C00[selected_player].kind=(seed+op+1)%9;
        if(op!=2)D_80395C00[selected_player].handle=(seed+op)%8;
    }
    if(mode==10 && op==8)D_80151AD0=(seed%4)+1;
}
void sound_call_minimal(s16 h) { record(1,h,0,0,0,0); }
s32 func_8008E398(s32 resource,f32 uv[3][3],s32 a,u32 b) {
    s32 i=(s32)(((char *)uv-(char *)D_80395C00-4)/(s32)sizeof(Status));
    record(2,resource,i,a,(s32)b,0);return ++token;
}
void func_8008E06C(s16 h,u32 *color) { record(3,h,(s32)*color,0,0,0); }
void model_data_load(s32 h,s32 a,u32 b) { record(4,h,a,(s32)b,0,0); }
void model_transform_setup(s32 h,s32 a,u32 b) { record(5,h,a,(s32)b,0,0); }
void func_8008B32C(f32 src[3][3],f32 dst[3][3],f32 scale) {
    s32 i=(s32)(((char *)src-(char *)D_80150B70)/(s32)sizeof(Camera));
    s32 j=(s32)(((char *)dst-(char *)D_80395C00-4)/(s32)sizeof(Status));
    record(6,i,j,bits(scale),0,0);dst[0][0]=src[0][0]*scale;
}
void func_80090E9C(f32 angle,f32 uv[3][3]) {
    s32 i=(s32)(((char *)uv-(char *)D_80395C00-4)/(s32)sizeof(Status));
    record(7,bits(angle),i,0,0,0);uv[2][2]=angle;
}
void car_lights_render(s32 i,s16 *point,Camera *camera,f32 scale,f32 *pos) {
    s32 j=(s32)(camera-D_80150B70);
    record(8,i,point[0],point[1],j,bits(scale));pos[0]=(f32)point[0];pos[1]=(f32)point[1];pos[2]=scale;
}
typedef struct Snapshot { Player players[4];Status statuses[4];Camera cameras[4];Scene scenes[16];Event events[256];s16 count;s32 nevent,token; } Snapshot;
static void save(Snapshot *s) {
    memset(s,0,sizeof(*s));
    memcpy(s->players,D_80152818,sizeof(D_80152818));memcpy(s->statuses,D_80395C00,sizeof(D_80395C00));
    memcpy(s->cameras,D_80150B70,sizeof(D_80150B70));memcpy(s->scenes,D_8012E700,sizeof(D_8012E700));
    memcpy(s->events,events,sizeof(events));s->count=D_80151AD0;s->nevent=nevent;s->token=token;
}
static void restore(const Snapshot *s) {
    memcpy(D_80152818,s->players,sizeof(D_80152818));memcpy(D_80395C00,s->statuses,sizeof(D_80395C00));
    memcpy(D_80150B70,s->cameras,sizeof(D_80150B70));memcpy(D_8012E700,s->scenes,sizeof(D_8012E700));
    memcpy(events,s->events,sizeof(events));D_80151AD0=s->count;nevent=s->nevent;token=s->token;
}
static void init(s32 count) {
    s32 i,j;
    memset(D_80152818,0x5a,sizeof(D_80152818));memset(D_80395C00,0,sizeof(D_80395C00));
    memset(D_80150B70,0,sizeof(D_80150B70));memset(D_8012E700,0,sizeof(D_8012E700));memset(events,0,sizeof(events));
    D_80151AD0=count;D_803942A4.word=0x4080c0ffu;D_80116178=0.5f;nevent=0;token=0;
    for(i=0;i<256;i++)D_801427C0[i]=(u16)(i+1000);
    for(i=0;i<4;i++) {
        D_80152818[i].kind=(seed+i*2)%9;D_80395C00[i].kind=(seed/9+i*3)%9;
        D_80395C00[i].handle=((seed/81+i)%3==0)?-1:(seed+i)%8;
        D_80393B74[i]=123+i;D_80150B70[i].uv[0][0]=(f32)(i+1);
        for(j=0;j<4;j++){D_803941D0[i][j].x=(s16)(10*i+j-20);D_803941D0[i][j].y=(s16)(20*j-i);}
    }
}
int main(void) {
    Snapshot before,a,b;s32 count;unsigned long cases=0;
    for(seed=0;seed<486;seed++)for(count=0;count<=4;count++)for(mode=0;mode<=10;mode++) {
        selected_player=seed%4;init(count);save(&before);
        if(func_80391864(NULL)!=1)fail("candidate return");save(&a);restore(&before);
        if(baseline_kind_scene(NULL)!=1)fail("baseline return");save(&b);
        if(memcmp(&a,&b,sizeof(a)))fail("state/event mismatch");cases++;
    }
    printf("PASS %lu actual-source kind-scene logical-host cases\n",cases);return 0;
}
