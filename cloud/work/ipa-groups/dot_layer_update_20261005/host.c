/* Host harness. The complete candidate is included unchanged. Only OS queues
 * and the allocator boundary use fixture definitions. Host Msg pointer layout
 * is intentionally native; outputs normalize entity/message pointer identity.
 */
#include <string.h>
#include <assert.h>
#include "candidate.c"
typedef unsigned int u32;
Entity fixture_entities[8];
Entity *D_80110244=fixture_entities;
s32 D_80146104=7;
Msg D_80142DD8[128];
s32 D_80142728,D_801427A8;
static LayerState fixture_layer;
static u32 mutations[12],mutation_count,event_count,lock_count,events[24];
static int locked,delivery_count,prefix;
static float to_float(u32 value) { float f; memcpy(&f,&value,4); return f; }
static u32 to_bits(float value) { u32 bits; memcpy(&bits,&value,4); return bits; }
static void event(u32 kind)
{
    u32 i;
    events[event_count*4]=kind;
    events[event_count*4+1]=(u32)fixture_layer.handle;
    events[event_count*4+2]=to_bits(fixture_layer.level);
    events[event_count*4+3]=to_bits(fixture_layer.style);
    event_count++;
    assert(event_count<=6);
    if(kind==3)return;
    lock_count++;
    for(i=0;i<mutation_count;i++)if(mutations[3*i]==lock_count) {
        if(mutations[3*i+1]==0)fixture_layer.handle=(s32)mutations[3*i+2];
        else if(mutations[3*i+1]==1)fixture_layer.level=to_float(mutations[3*i+2]);
        else if(mutations[3*i+1]==2)fixture_layer.style=to_float(mutations[3*i+2]);
        else assert(0);
    }
}
s32 osRecvMesg(void *q,void *msg,s32 block)
{
    assert(q==&D_80142728 && msg==0 && block==1 && !locked);
    locked=1;event(1);return 0;
}
s32 osJamMesg(void *q,void *msg,s32 block)
{
    assert(block==0 && locked);
    if(q==&D_80142728) { assert(msg==0);locked=0;event(2); }
    else { assert(q==&D_801427A8 && msg==D_80142DD8+prefix+delivery_count);delivery_count++;event(3); }
    return 0;
}
Msg *func_80091B00(void)
{
    s32 i;
    for(i=0;i<128;i++)if(D_80142DD8[i].used==0) {
        D_80142DD8[i].used=1;D_80142DD8[i].id=-1;return &D_80142DD8[i];
    }
    return 0;
}
__attribute__((visibility("default"))) u32 run_case(const u32 *in,u32 *out)
{
    u32 i,j,k=0,cursor=0,count_pos;
    float level,style;
    memset(&fixture_layer,0xa5,sizeof(fixture_layer));
    fixture_layer.handle=(s32)in[cursor++];fixture_layer.level=to_float(in[cursor++]);fixture_layer.style=to_float(in[cursor++]);
    level=to_float(in[cursor++]);style=to_float(in[cursor++]);prefix=(int)in[cursor++];
    assert(prefix>=0 && prefix<=126);
    memset(fixture_entities,0xb6,sizeof(fixture_entities));
    for(i=0;i<8;i++) { fixture_entities[i].id=256+i;fixture_entities[i].msgCount=(u8)in[cursor++]; }
    for(i=0;i<8;i++) {
        fixture_entities[i].x=to_float(in[cursor++]);fixture_entities[i].y=to_float(in[cursor++]);
        fixture_entities[i].z=to_float(in[cursor++]);fixture_entities[i].w=to_float(in[cursor++]);
    }
    mutation_count=in[cursor++];assert(mutation_count<=4);
    for(i=0;i<3*mutation_count;i++)mutations[i]=in[cursor++];
    memset(D_80142DD8,0xa5,sizeof(D_80142DD8));
    for(i=0;i<128;i++)D_80142DD8[i].used=(s8)(i<(u32)prefix);
    event_count=lock_count=0;locked=delivery_count=0;
    mode_select_input(&fixture_layer,level,style);
    assert(!locked);
    out[k++]=(u32)fixture_layer.handle;out[k++]=to_bits(fixture_layer.level);out[k++]=to_bits(fixture_layer.style);
    out[k++]=0xa5a5a5a5U;out[k++]=0xa5a5a5a5U;
    for(i=0;i<8;i++)out[k++]=fixture_entities[i].msgCount;
    count_pos=k++;out[count_pos]=0;
    for(i=prefix;i<128;i++)if(D_80142DD8[i].used) {
        Msg *m=&D_80142DD8[i];out[count_pos]++;
        out[k++]=i;out[k++]=(unsigned short)m->id;out[k++]=(u8)m->type;out[k++]=(u8)m->used;
        out[k++]=to_bits(m->payload.value.x);out[k++]=to_bits(m->payload.value.y);out[k++]=to_bits(m->payload.value.z);out[k++]=to_bits(m->payload.value.w);
        out[k++]=(u32)(m->ent-fixture_entities);
    }
    assert(out[count_pos]==(u32)delivery_count);
    out[k++]=event_count;
    for(j=0;j<4*event_count;j++)out[k++]=events[j];
    return k;
}
