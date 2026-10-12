/* Actual-source finite host comparison. Native byte identity is proved separately.
 * Host structure layout is logical; no cross-endian or gameplay claim is made. */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
/* Host-only width adapter for the native u32 byte count. */
void *packet_memset(void *p,s32 x,u32 n) { return memset(p,x,n); }

s16 D_8014A108;
Player D_8014A118[4];
Profile *D_80146150[4];
Profile **D_8012E6E0,**D_8012E6E4,**D_803BAC80[4];
void *D_803BAC68[4];
s32 D_803BACE0[4],D_803BAD80[4],D_803BAC20[4],D_803BA888[4],D_803B5ED4;
s8 D_803BADB8[4],D_803BADBC[4],D_803BADC0[4],D_803BADC8[4],D_803BADCC[4],D_803BADD0[4],D_803BADD4[4],D_803BAD78[4],D_8015418C[4];
s8 D_803BAD00[4][14],D_803BAD40[4][14];
u8 D_803BAC98[4][13];
s16 D_803BACD0[4],D_803BACD8[4];
ListRoot D_8012E6D8;
static Profile profiles[8],defaults[4];
static Profile *handles[8];
static FileInfo files[8];
static FileInfo *file_handles[8];
static int check_result;
typedef struct Event { int kind,slot;u32 argument; } Event;
static Event events[64];static unsigned event_count;
static int index_of(Profile **p) {
    int i;
    if (!p) return -1;
    for(i=0;i<8;i++)if(p==&handles[i])return i;
    for(i=0;i<4;i++)if(p==&D_80146150[i])return 8+i;
    fprintf(stderr,"invalid owner\n");exit(3);
}
static void event(int kind,int slot,u32 argument) {
    if(event_count>=64)exit(4);
    events[event_count].kind=kind;events[event_count].slot=slot;events[event_count++].argument=argument;
}
void resource_type_select(u32 x) { event(1,0,x); }
void audio_doppler(u32 x) { event(2,0,x); }
void audio_distance_atten(u32 x) { event(3,0,x); }
s32 func_800CCEFC(Profile **p) { event(4,index_of(p),0);return check_result; }
void func_800A3424(FileInfo **p,s32 active) {
    int i,slot=-1;
    for(i=0;i<8;i++)if(p==&file_handles[i])slot=i;
    event(5,slot,(u32)active);
}
#define STATE_FIELDS(X) \
 X(D_8014A118) X(D_803BAC80) X(D_803BAC68) X(D_803BACE0) X(D_803BAD80) \
 X(D_803BAC20) X(D_803BA888) X(D_803BADB8) X(D_803BADBC) X(D_803BADC0) \
 X(D_803BADC8) X(D_803BADCC) X(D_803BADD0) X(D_803BADD4) X(D_803BAD78) \
 X(D_8015418C) X(D_803BAD00) X(D_803BAD40) X(D_803BAC98) X(D_803BACD0) X(D_803BACD8)
#define FIELD(n) unsigned char n[sizeof(n)];
typedef struct Snapshot { STATE_FIELDS(FIELD) s32 result;Event trace[64];unsigned calls; } Snapshot;
#undef FIELD
static Snapshot snapshot(void) {
    Snapshot result;memset(&result,0,sizeof(result));
#define COPY(n) memcpy(result.n,n,sizeof(n));
    STATE_FIELDS(COPY)
#undef COPY
    result.result=D_803B5ED4;result.calls=event_count;memcpy(result.trace,events,sizeof(events));return result;
}
static void setup(int count,int player,int selection,int input,int scenario) {
    int i,j;static const u32 pressed[]={0,1,2,4,3,6,7};
    static const u32 repeats[]={0,0x2000,0x1000,0x400,0x800,0x3000,0xc00};
#define ZERO(n) memset(n,0,sizeof(n));
    STATE_FIELDS(ZERO)
#undef ZERO
    memset(events,0,sizeof(events));event_count=0;D_803B5ED4=0;D_8014A108=count;
    for(i=0;i<8;i++) {
        memset(&profiles[i],0,sizeof(profiles[i]));memset(&files[i],0,sizeof(files[i]));
        handles[i]=&profiles[i];file_handles[i]=&files[i];files[i].controller=i%4;
        profiles[i].previous=i?&handles[i-1]:0;profiles[i].next=i<7?&handles[i+1]:0;profiles[i].file=&file_handles[i];
    }
    D_8012E6E0=D_8012E6D8.head=&handles[0];D_8012E6E4=D_8012E6D8.tail=&handles[7];
    for(i=0;i<4;i++) {
        memset(&defaults[i],0,sizeof(defaults[i]));defaults[i].file=&file_handles[i];D_80146150[i]=&defaults[i];
        D_8014A118[i].id=i;D_803BADBC[i]=(i+scenario)%4;D_803BAC20[i]=2;D_803BADB8[i]=(scenario>>1)&1;
        D_803BACE0[i]=i==player?0:(scenario+i)%5;
        if(i!=player && scenario&1)D_8014A118[i].owner=&handles[i];
        D_803BAD80[i]=i==player?selection:0;
        D_803BAC80[i]=(i==player && selection>=3 && selection<11)?&handles[(scenario+selection)%8]:0;
        for(j=0;j<14;j++)D_803BAD00[i][j]=j==0 || j==1 || (j>=3 && j<=12 && ((j+scenario)%4!=0));
        D_803BAD00[i][13]=(scenario>>2)&1;
        D_803BACD0[i]=5;D_803BACD8[i]=2;memset(D_803BAC98[i],'x',13);
    }
    D_8014A118[player].pressed=pressed[input%7];D_8014A118[player].repeat=repeats[input/7];check_result=(scenario>>2)&1;
}
int main(void) {
    int count,player,row,input,scenario;unsigned cases=0;
    for(count=1;count<=4;count++)for(player=0;player<count;player++)for(row=0;row<14;row++)
    for(input=0;input<49;input++)for(scenario=0;scenario<8;scenario++) {
        Snapshot want,got;
        setup(count,player,row,input,scenario);baseline_controller(player);want=snapshot();
        setup(count,player,row,input,scenario);func_80396FA0(player);got=snapshot();
        if(memcmp(&want,&got,sizeof(want))) { fprintf(stderr,"case %d %d %d %d %d differs\n",count,player,row,input,scenario);return 1; }
        cases++;
    }
    printf("{\"cases\":%u,\"complete_state_and_events_equal\":true,\"helper_contract\":\"deterministic synchronous doubles\"}\n",cases);
    return 0;
}
