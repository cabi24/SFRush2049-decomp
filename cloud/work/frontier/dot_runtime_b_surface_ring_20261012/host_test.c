/* Finite logical-host comparison of the actual candidate and inherited source. */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "group.c"
struct Record88 { int id; };
struct Poly { int unused; };
Vehicle D_8014A250[4];
RingState D_80399A70;
u16 D_80399AD8 = 23;
f32 D_80394888[4][3];
u8 D_803948B8[5][4];
static Record88 objects[26];
static Poly polygon;
static int mask, geometry, calls, count;
static struct Event { int kind,id,a,b,c; float position[12]; } events[16];
void baseline_func(f32 *,s32);
static void check(int x) { if (!x) { fprintf(stderr,"host invariant failed\n"); exit(1); } }
static void event(int kind, Record88 *object) {
    check(count < 16); memset(&events[count],0,sizeof events[count]);
    events[count].kind=kind; events[count].id=object ? object->id : -1; count++;
}
Poly *camera_trigger_check(f32 *position,f32 *hit,f32 normal[3][3]) {
    int i, n=calls++;
    event(1,0); memcpy(events[count-1].position,position,3*sizeof(float));
    for(i=0;i<3;i++) hit[i]=position[i];
    memset(normal,0,9*sizeof(float));
    if (geometry>=1 && geometry<=4 && n==geometry-1) hit[1]+=4.0f;
    if (geometry>=5 && geometry<=8 && n==geometry-5) hit[0]+=15.0f;
    return mask & (1<<n) ? &polygon : 0;
}
void func_8008C874(Record88 *object) { check(object!=0); event(2,object); }
Record88 *func_800A78BC(s32 n,f32 *position,u16 texture,u8 *color,u16 flags,s32 mode) {
    check(n==4 && position==&D_80394888[0][0] && texture==23 && color==D_803948B8[0] && flags==0x820f && mode==1);
    event(3,&objects[25]); return &objects[25];
}
void func_8008C074(Record88 *object,s32 mode,f32 *position,u16 texture,u8 *color,u16 flags,s32 last) {
    check(object && mode==0 && texture==0 && last==0);
    event(4,object); events[count-1].a=(int)(color-(u8 *)&D_803948B8); events[count-1].b=flags;
    if(position) memcpy(events[count-1].position,position,12*sizeof(float));
}
int main(void) {
    int player,compression,m,g,index,wrapped,nulls,i,which,total=0;
    RingState initial, expected;
    struct Event expected_events[16];
    int expected_count=0,expected_calls=0;
    f32 position[3]={13.5f,-2.25f,7.75f};
    for(i=0;i<26;i++) objects[i].id=i;
    for(player=0;player<4;player++) for(compression=0;compression<6;compression++)
    for(m=0;m<16;m++) for(g=0;g<9;g++) for(index=0;index<25;index++)
    for(wrapped=0;wrapped<2;wrapped++) for(nulls=0;nulls<2;nulls++) {
        memset(&initial,0,sizeof initial); initial.next=index; initial.wrapped=wrapped;
        for(i=0;i<25;i++) initial.objects[i]=(nulls && i%3==0) ? 0 : &objects[i];
        if(wrapped) initial.objects[index]=&objects[index];
        memset(D_8014A250,0,sizeof D_8014A250);
        if(compression==1) for(i=0;i<4;i++) D_8014A250[player].compression[i]=8.0f;
        if(compression>=2) D_8014A250[player].compression[compression-2]=8.25f;
        for(which=0;which<2;which++) {
            D_80399A70=initial; mask=m; geometry=g; count=calls=0; memset(events,0,sizeof events);
            if(which) func_8038A408(position,player); else baseline_func(position,player);
            check(position[0]==13.5f && position[1]==-2.25f && position[2]==7.75f);
            if(!which) { expected=D_80399A70; expected_count=count; expected_calls=calls; memcpy(expected_events,events,sizeof events); }
            else { check(memcmp(&expected,&D_80399A70,sizeof expected)==0); check(expected_count==count && expected_calls==calls); check(memcmp(expected_events,events,sizeof events)==0); }
            if(compression>=2) check(count==0 && calls==0);
        }
        total++;
    }
    printf("%d finite actual-source equivalence cases\n",total);
    return 0;
}
