/* Explicit callback contracts, C89 host proof. */
#include <assert.h>
#include <string.h>
#include <stddef.h>
#ifndef CANDIDATE_SOURCE
#define CANDIDATE_SOURCE "candidate.c"
#endif
#include CANDIDATE_SOURCE
Descriptor D_80117530[3];
Car D_80152818[2];
Model D_8014A250[2];
s8 D_8013FECC, D_8013FECD;
Visual *D_801391F0;
static Target target;
static Motion motion, replacement;
static Visual visual, old_head, changed_head;
static unsigned *out;
static unsigned failure, mutation;
static unsigned bits(float f) { unsigned n; memcpy(&n,&f,4); return n; }
static float value(unsigned n) { float f; memcpy(&f,&n,4); return f; }
static void event(unsigned n) { unsigned k=out[26]++; assert(k<10);out[27+k]=n; }
static void callback0(void *p,s16 x) {(void)p;(void)x;}
static void callback1(void *p,s16 x) {(void)p;(void)x;}
Visual *func_80090284(void) {
    event(3);
    if(mutation&1) {target.owner=1;target.descriptor=2;}
    return failure ? 0 : &visual;
}
void model_data_load(s32 object,s32 a,u32 b) {assert(object==-17 && a==0 && b==15);event(1);}
void save_write_data(void *p,s32 a,f32 x,s32 b) {assert(p==target.state && !a && bits(x)==bits(0.4f) && b==1);event(2);}
void vector_normalize_length(f32 *v,f32 m[3][3]) {
    int i,j; assert(v==motion.velocity);event(4);
    for(i=0;i<3;i++)for(j=0;j<3;j++)m[i][j]=(float)(i*3+j+1)+v[j];
    if(mutation&2) {target.type=339;target.owner=1;target.descriptor=2;target.motion=&replacement;D_801391F0=&changed_head;}
}
void math_utility(void *input,void *output) {
    int i,j;f32 (*a)[3]=input;f32 (*b)[3]=output;
    assert(output==target.orientation);event(5);
    for(i=0;i<3;i++)for(j=0;j<3;j++)b[i][j]=a[i][j];
}
s32 stat_lap_split(s32 sound,s32 owner,f32 *position,u8 style) {
    assert(position==target.position && style==2);event(6);out[40]=(unsigned)sound;out[41]=(unsigned)owner;return -1;
}
void host_case(const unsigned *in,unsigned *output) {
    int i,j;out=output;memset(out,0,64*sizeof(*out));
    memset(&target,0,sizeof(target));memset(&motion,0,sizeof(motion));memset(&replacement,0,sizeof(replacement));
    memset(&visual,0,sizeof(visual));memset(D_80117530,0,sizeof(D_80117530));
    memset(D_80152818,0,sizeof(D_80152818));memset(D_8014A250,0,sizeof(D_8014A250));
    D_8013FECD=(s8)in[0];D_8013FECC=(s8)in[1];D_80117530[1].type=(s16)in[2];
    target.type=(s16)in[3];target.flags=(u8)in[4];D_80152818[0].hit=(s8)in[5];D_8014A250[0].hit=(s8)in[6];
    failure=in[7];mutation=in[8];target.descriptor=1;target.owner=0;target.motion=&motion;target.object=-17;
    D_80117530[1].callback=callback0;D_80117530[2].callback=callback1;
    D_80117530[1].sound=123;D_80117530[2].sound=456;D_801391F0=&old_head;
    for(i=0;i<3;i++) {D_80152818[0].velocity[i]=value(in[9+i]);motion.angle[i]=value(in[12+i]);motion.velocity[i]=value(in[15+i]);}
    for(i=0;i<3;i++)for(j=0;j<3;j++)target.orientation[i][j]=value(in[18+i*3+j]);
    func_8010DCFC(&target);
    out[0]=target.flags;out[1]=(unsigned)(u8)D_80152818[0].hit;out[2]=(unsigned)(u8)D_80152818[1].hit;
    out[3]=(unsigned)(u8)D_8014A250[0].hit;out[4]=(unsigned)(u8)D_8014A250[1].hit;
    out[5]=visual.next==&old_head?1:visual.next==&changed_head?2:0;out[6]=(unsigned)(unsigned short)visual.index;
    out[7]=visual.target==&target;out[8]=bits(visual.lifetime);out[9]=visual.callback==callback0?1:visual.callback==callback1?2:0;
    out[10]=D_801391F0==&visual?3:D_801391F0==&changed_head?2:1;
    for(i=0;i<3;i++){out[11+i]=bits(motion.angle[i]);out[14+i]=bits(motion.velocity[i]);}
    for(i=0;i<3;i++)for(j=0;j<3;j++)out[17+i*3+j]=bits(target.orientation[i][j]);
    out[37]=(unsigned)(u8)target.owner;out[38]=(unsigned)(unsigned short)target.descriptor;out[39]=(unsigned)(unsigned short)target.type;
    out[42]=target.motion==&replacement;
    for(i=0;i<3;i++)assert(bits(replacement.angle[i])==0 && bits(replacement.velocity[i])==0);
}
