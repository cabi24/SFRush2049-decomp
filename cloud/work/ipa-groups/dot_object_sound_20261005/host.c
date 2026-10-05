/* Host ABI adapter. Candidate and accepted allocator/yaw sources are unchanged. */
#include <assert.h>
#include <stddef.h>
#include <string.h>
#ifndef CANDIDATE
#define CANDIDATE "candidate.c"
#endif
#include CANDIDATE

Player player_array[8];
Definition D_80117530[16];
Node *D_801391F0, *D_801392C8;
s16 D_8012E66C, D_8012E678;
static Actor actor;
static Node node, old_head, next_free;
static u32 captured[7], sound_calls;

static u32 bits(f32 x) {u32 n; memcpy(&n,&x,4); return n;}

s32 stat_lap_split(s32 sound,s32 player,f32 *position,u8 mode)
{
    sound_calls++;
    captured[0]=(u32)sound; captured[1]=(u32)player;
    captured[2]=(u32)mode;
    captured[3]=bits(position[0]); captured[4]=bits(position[1]); captured[5]=bits(position[2]);
    captured[6]=(D_801391F0==&node && node.next==&old_head && node.fieldC==&actor);
    return -12345;
}

void run_case(s32 available,s32 player,s32 index,u32 flags,const f32 *values,u32 *out)
{
    Player players_before[8];
    Definition defs_before[16];
    Actor original;
    s32 i;
    assert(sizeof(Actor)==96 && offsetof(Actor,state)==90 && offsetof(Actor,player)==92);
    assert(sizeof(Player)==952 && sizeof(Definition)==48);
    assert(player>=0 && player<8 && index>=0 && index<16);
    memset(&actor,0xa5,sizeof(actor)); memset(&node,0x6b,sizeof(node));
    memset(&old_head,0x79,sizeof(old_head)); memset(&next_free,0x53,sizeof(next_free));
    memset(player_array,0x3c,sizeof(player_array)); memset(D_80117530,0x17,sizeof(D_80117530));
    actor.flags=(u8)flags; actor.index=(s16)index; actor.player=(s8)player;
    for(i=0;i<9;i++) actor.matrix[i/3][i%3]=values[i];
    for(i=0;i<3;i++) {player_array[player].velocity[i]=values[9+i]; actor.position[i]=values[12+i];}
    for(i=0;i<16;i++) {D_80117530[i].stat=10000+i*73;D_80117530[i].sound=-2000+i*31;}
    node.next=&next_free;
    D_801392C8=available?&node:0; D_801391F0=&old_head;
    D_8012E66C=123;D_8012E678=123;
    sound_calls=0;memset(captured,0,sizeof(captured));
    memcpy(players_before,player_array,sizeof(player_array));memcpy(defs_before,D_80117530,sizeof(D_80117530));
    original=actor;
    func_8010DBB8(&actor);
    assert(memcmp(players_before,player_array,sizeof(player_array))==0);
    assert(memcmp(defs_before,D_80117530,sizeof(D_80117530))==0);
    assert(memcmp(actor.opaque0,original.opaque0,4)==0 && memcmp(actor.opaque5,original.opaque5,11)==0);
    assert(memcmp(actor.opaque18,original.opaque18,2)==0 && memcmp(actor.opaque68,original.opaque68,22)==0);
    assert(actor.index==original.index && actor.player==original.player);
    assert(memcmp(actor.position,original.position,12)==0);
    out[0]=(u32)actor.state;out[1]=actor.flags;
    for(i=0;i<9;i++) out[2+i]=bits(actor.matrix[i/3][i%3]);
    out[11]=sound_calls;
    for(i=0;i<7;i++)out[12+i]=captured[i];
    out[19]=(D_801391F0==&node);out[20]=(D_801392C8==&next_free);
    out[21]=(u32)D_8012E66C;out[22]=(u32)D_8012E678;
    out[23]=available?(u32)node.field4:0;
    out[24]=available?(u32)node.field14:0;
    out[25]=available?bits(node.field10):0;
    out[26]=available?(u32)(node.fieldC==&actor):0;
    out[27]=available?(u32)(node.next==&old_head):0;
    out[28]=available?(u32)(node.field6==-1 && node.field8==0 && node.padA[0]==0x6b && node.padA[1]==0x6b):0;
}
