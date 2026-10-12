/* Actual root bodies, deterministic helper doubles, stable logical host state.
 * This tests source equivalence, not native pointer widths or real helpers. */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

s16 D_8014A108,D_80151AD0;
Player D_8014A118[4];
u8 D_8012E67C[4];
s32 D_803BAD80[4],D_803B5CFC[5][24];
s8 D_803BACF4,D_803B5ED0;
f32 D_803B5D0C,D_803B5D10,D_803B5D14,D_803B5D18,D_803B5D1C,D_803B5D20,D_803B5D24,D_803B5D28;
struct FiveParts {s32 id;};struct Blit {s32 id;};
FiveParts *D_803BAC08[4];MultiBlit D_803B5D2C[2];Blit *D_803B5ED8;s32 D_803B5ED4;
static Owner owners[4];static OwnerBody bodies[4];static Selection selections[4];static Item items[4];
static FiveParts handles[4];static Blit blit;
static int events[128][10],event_count,selected_font,part_index,seed_value,denominator;
static void event(int id,int a,int b) {
    if(event_count>=128)abort();events[event_count][0]=id;events[event_count][1]=a;events[event_count][2]=b;event_count++;
}
void func_800A3424(void **p,s8 arg) {
    int i;for(i=0;i<4;i++)if(p==(void **)&selections[i])break;if(i==4)abort();event(1,i,arg);
}
void func_80394834(void){event(2,0,0);}
void func_80394B00(s32 a,s32 b){event(3,a,b);}
void *object_create(s32 n){selected_font=n;event(4,n,0);return NULL;}
s16 object_bytes_sum_global(void){int r=selected_font==11||selected_font==13?seed_value+7:denominator;event(5,selected_font,r);return r;}
void func_80397AD8(s8 n){event(6,n,0);}
void func_80394564(s32 n){event(7,n,0);}
void func_800A6404(void){event(8,0,0);}
void particle_lifetime_set(void){event(9,0,0);}
FiveParts *ambient_sound_set(s32 a,s32 b,s32 c,s32 d,s32 e,s32 f,s32 g,s32 h) {
    int i=part_index++,row=event_count;if(i>=4)abort();event(10,i,0);
    events[row][2]=a;events[row][3]=b;events[row][4]=c;events[row][5]=d;
    events[row][6]=e;events[row][7]=f;events[row][8]=g;events[row][9]=h;return &handles[i];
}
Blit *sound_control(s16 a,s16 b,const MultiBlit *c,s16 d) {if(c!=D_803B5D2C)abort();event(11,a,b);events[event_count-1][3]=d;return &blit;}
void entity_audio_update(void){event(12,0,0);}

typedef struct Snapshot {
    Player players[4];u8 ids[4];s32 choices[4];s16 screens;s8 rows,active;
    f32 geometry[8];FiveParts *parts[4];Blit *root;s32 state;
    int count,events[128][10];
} Snapshot;
static void setup(int count,int mask,int seed,int denom) {
    int i,j;memset(D_8014A118,0,sizeof(D_8014A118));memset(events,0,sizeof(events));
    event_count=selected_font=part_index=0;seed_value=seed;denominator=denom;D_8014A108=count;
    for(i=0;i<4;i++){
        owners[i].body=&bodies[i];bodies[i].selection=&selections[i];selections[i].item=&items[i];
        D_8014A118[i].id=15+i;D_8014A118[i].owner=(mask&(1<<i))?&owners[i]:NULL;
        D_8012E67C[i]=20+i;D_803BAD80[i]=-50-i;D_803BAC08[i]=NULL;
    }
    for(i=0;i<5;i++)for(j=0;j<24;j++)D_803B5CFC[i][j]=seed*37+i*13-255+j;
    D_80151AD0=-5;D_803BACF4=-23;D_803B5ED0=-1;D_803B5ED4=-123;D_803B5ED8=NULL;
    D_803B5D0C=D_803B5D10=D_803B5D14=D_803B5D18=D_803B5D1C=D_803B5D20=D_803B5D24=D_803B5D28=-99.0f;
}
static Snapshot take(void) {
    Snapshot r;memset(&r,0,sizeof(r));memcpy(r.players,D_8014A118,sizeof(r.players));memcpy(r.ids,D_8012E67C,sizeof(r.ids));
    memcpy(r.choices,D_803BAD80,sizeof(r.choices));r.screens=D_80151AD0;r.rows=D_803BACF4;r.active=D_803B5ED0;
    r.geometry[0]=D_803B5D0C;r.geometry[1]=D_803B5D10;r.geometry[2]=D_803B5D14;r.geometry[3]=D_803B5D18;
    r.geometry[4]=D_803B5D1C;r.geometry[5]=D_803B5D20;r.geometry[6]=D_803B5D24;r.geometry[7]=D_803B5D28;
    memcpy(r.parts,D_803BAC08,sizeof(r.parts));r.root=D_803B5ED8;r.state=D_803B5ED4;r.count=event_count;memcpy(r.events,events,sizeof(events));return r;
}
int main(void) {
    int n,mask,seed,denom,cases=0;Snapshot old,new_;
    for(n=0;n<=4;n++)for(mask=0;mask<16;mask++)for(seed=0;seed<16;seed++)for(denom=1;denom<=16;denom++){
        setup(n,mask,seed,denom);baseline_root();old=take();setup(n,mask,seed,denom);func_80397FFC();new_=take();
        if(memcmp(&old,&new_,sizeof(old)))abort();cases++;
    }
    if(cases!=20480)abort();puts("20480 actual-root logical-host equivalence cases");return 0;
}
