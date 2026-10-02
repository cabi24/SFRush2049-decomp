#include <assert.h>
#include <stdint.h>
#include <string.h>
#include "candidate.c"
s32 D_801170FC;
u8 D_80140BDC;
u8 D_80121DA0[4],D_80121DA4[4],D_80121DA8[4];
s32 D_80118E08[3];
ResourceSlot D_8012E738[8];
static ResourceNode node;
static ResourceState state;
static u8 text[16],found[8];
static uint32_t input[11],trace[5],ncalls;
void audio_channel_setup(void) { assert(0); }
void entity_transform_apply(ResourceNode *p,s32 n) {assert(p==&node && n==1);trace[ncalls++]=0;}
u8 *func_800A464C(u8 *p,u8 *pattern) {
    unsigned which=pattern==D_80121DA0?0:pattern==D_80121DA4?1:2;
    assert(pattern==D_80121DA0 || pattern==D_80121DA4 || pattern==D_80121DA8);
    assert(p==text+(which==2?8:0) && state.timer==10);
    trace[ncalls++]=10+which;
    return input[3+which]?found:0;
}
s32 sound_bank_load(s32 resource,s16 *id,s8 first,s8 last,s32 mode) {
    unsigned kind=input[3]?2:input[4]?1:0;
    assert(resource==D_80118E08[kind] && first==0 && last==(s8)(input[7]-1) && mode==1);
    *id=0x1234;trace[ncalls++]=20+kind;
    state.slot=(s16)input[9];state.text=text+8;state.flags=(u8)(input[2]^0x80);
    return (s32)input[10];
}
void run(const uint32_t *x,uint32_t *out) {
    unsigned i;ResourceNode before_node;ResourceState before_state;ResourceSlot expected[8];
    memcpy(input,x,sizeof(input));ncalls=0;
    memset(&node,0xa5,sizeof(node));memset(&state,0xa5,sizeof(state));memset(D_8012E738,0xa5,sizeof(D_8012E738));
    node.variant=0x5432;node.state=&state;node.time=1.0f;node.callback=0;
    state.flags=(u8)x[2];state.slot=(s16)x[8];state.timer=0x4321;state.text=text;
    D_801170FC=(s32)x[1];D_80140BDC=(u8)x[7];found[4]=(u8)x[6];
    for(i=0;i<3;i++)D_80118E08[i]=(s32)(0x67800000u+i);
    before_node=node;before_state=state;memcpy(expected,D_8012E738,sizeof(expected));
    func_8010D85C(&node,(s16)x[0]);
    if((s16)x[0] && !x[1]) {
        before_node.time=0.0f;before_node.callback=audio_channel_setup;
        if(!(x[2]&1)) {
            before_node.variant=x[5]?(s8)(x[6]-48):0;
            before_state.flags=(u8)((x[2]^0x80)|1);before_state.timer=10;
            before_state.slot=(s16)x[9];before_state.text=text+8;
            expected[x[9]].handle=(s32)x[10];
        }
    }
    assert(memcmp(&before_node,&node,sizeof(node))==0);
    assert(memcmp(&before_state,&state,sizeof(state))==0);
    assert(memcmp(expected,D_8012E738,sizeof(expected))==0);
    out[0]=(uint16_t)node.variant;memcpy(out+1,&node.time,4);out[2]=node.callback==audio_channel_setup;
    out[3]=state.flags;out[4]=(uint16_t)state.timer;out[5]=(uint16_t)state.slot;
    for(i=0;i<8;i++)out[6+i]=(uint32_t)D_8012E738[i].handle;
    out[14]=ncalls;for(i=0;i<ncalls;i++)out[15+i]=trace[i];
}
#ifdef HOST_MAIN
static uint32_t random_state=0x8010d85c;
static uint32_t next(void){random_state^=random_state<<13;random_state^=random_state>>17;random_state^=random_state<<5;return random_state;}
int main(void){uint32_t x[11],out[20];unsigned i,n;for(n=0;n<100000;n++){for(i=0;i<11;i++)x[i]=next();x[1]&=1;x[2]&=255;x[3]&=1;x[4]&=1;x[5]&=1;x[6]&=255;x[7]&=255;x[8]&=7;x[9]&=7;run(x,out);}return 0;}
#endif
