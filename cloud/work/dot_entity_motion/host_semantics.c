/* Host callback-contract harness for the unmodified research C. */
#include <assert.h>
#include <string.h>
#include <stddef.h>
#include "func_8010E4E4.c"
s32 D_801170FC;
f32 D_8002EB94, D_801249CC, D_80121DDC[3];
Kind D_80117530[4];
u8 D_80143FC8[4];
static State state;
static Object object, replacement;
static Model model;
static unsigned int *result;
static int mutate;
static unsigned int bits(float f) { unsigned int u; memcpy(&u,&f,4); return u; }
static float value(unsigned int u) { float f; memcpy(&f,&u,4); return f; }
static void event(unsigned int id) { unsigned int n=result[13]++; assert(n<4); result[14+n]=id; }
void entity_transform_apply(void *p,s32 mode) { assert(p==&state && mode==1); event(4); }
void entity_spawn_callback(s32 index,s32 a,s32 b) { assert(!a&&!b); result[18]=(unsigned int)index; event(2); }
void func_800AFA84(void *p,void *o) { assert(p==D_80143FC8 && o==&object); event(3); }
void sound_position_set(f32 *p,void *out) {
    int i; assert(out==object.position); event(1);
    for(i=0;i<3;i++) { result[9+i]=bits(p[i]); object.position[i]=p[i]; }
    if(mutate) { state.timer=2.0f; D_8002EB94=4.0f; object.kind=3; object.index=-12; state.obj=&replacement; }
}
/* in: nine vector bits, timer, dt, rate, acceleration[3], mode, pause,
 * kind flags, callback mutation. out: vectors, timer, callback vector,
 * final dt, event count/event ids, spawn index, cached-object assertion. */
void host_case(const unsigned int *input,unsigned int *output) {
    int i;
    memset(&state,0,sizeof(state)); memset(&object,0,sizeof(object));
    memset(&model,0,sizeof(model)); memset(&replacement,0,sizeof(replacement));
    memset(output,0,20*sizeof(*output)); memset(D_80117530,0,sizeof(D_80117530));
    result=output; mutate=(int)input[18]; state.obj=&object; object.model=&model;
    object.kind=2; object.index=-7; D_80117530[2].flags=(u16)input[17]; D_80117530[3].flags=0x2000;
    for(i=0;i<3;i++) { model.pos[i]=value(input[i]);model.vel[i]=value(input[3+i]);object.accumulated[i]=value(input[6+i]);D_80121DDC[i]=value(input[12+i]); }
    state.timer=value(input[9]); D_8002EB94=value(input[10]); D_801249CC=value(input[11]);
    D_801170FC=(s32)input[16];
    func_8010E4E4(&state,(s16)input[15]);
    for(i=0;i<3;i++) { output[i]=bits(model.vel[i]);output[3+i]=bits(object.accumulated[i]);output[6+i]=bits(object.position[i]); }
    output[12]=bits(state.timer);output[19]=bits(D_8002EB94);
}
#ifdef STANDALONE
int main(void) {
    unsigned int input[19],output[20],state=0x13579bdfU;
    int i,n;
    assert(offsetof(Kind,flags)==18 && sizeof(Kind)==48);
    for(n=0;n<50000;n++) {
        for(i=0;i<19;i++) { state=state*1664525U+1013904223U;input[i]=state; }
        input[15]=n%4==0?0:1;input[16]=n%5==0?1:0;input[18]=n%3==0;
        host_case(input,output);
    }
    return 0;
}
#endif
