/* Bounded state/call differential tests, plus the restored return contract. */
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "candidate.c"
#include "historical_body.inc"

Descriptor72 D_8017A510[4];
Viewport16 D_8011EA30;
f32 D_80151AA0;
u32 D_80124FC8;
static int objects[8];
static unsigned calls, fill_calls;
static int call_index;
static f32 call_args[6];
static Descriptor72 pre_call[4];

void func_800A5560(u16 color) {
    D_80124FC8 = ((u32)color << 16) | (u32)color;
    fill_calls++;
}
/* Bounded stand-in: records the genuine seven-argument boundary and models
 * its directly written projection fields. Exact native helper code is checked
 * separately; this host test does not claim a trigonometric implementation. */
Descriptor72 *exhaust_smoke_effect(int idx, f32 h, f32 v, f32 width,
                                  f32 height, f32 hc, f32 vc) {
    Descriptor72 *p=&D_8017A510[idx];
    memcpy(pre_call,D_8017A510,sizeof pre_call);
    call_index=idx;call_args[0]=h;call_args[1]=v;call_args[2]=width;
    call_args[3]=height;call_args[4]=hc;call_args[5]=vc;calls++;
    p->width=width;p->height=height;p->near_plane=hc;p->far_plane=vc;
    p->horizontal=h;p->vertical=v;p->aspect=width/height;
    p->orthographic=h>0.0f?0:1;
    return p;
}
static void fail(void) { fputs("window-initializer differential mismatch\n",stderr);exit(3); }
static void reset(unsigned n) {
    unsigned i;
    memset(D_8017A510,0,sizeof D_8017A510);memset(&D_8011EA30,0,sizeof D_8011EA30);
    memset(pre_call,0,sizeof pre_call);memset(call_args,0,sizeof call_args);
    D_80151AA0=-3.5f;D_80124FC8=0x12345678u;calls=fill_calls=0;call_index=-1;
    for(i=0;i<4;i++) {
        Descriptor72 *p=&D_8017A510[i];
        p->first=&objects[i];p->bounds=&objects[i+4];p->width=(float)(n%501+i);
        p->height=(float)(n%313+i+1);p->zoom=-8.0f;p->fog=-9.0f;
        p->range_start=-1;p->range_end=-2;p->red=123;p->green=45;p->blue=67;p->alpha=89;
    }
    for(i=0;i<4;i++){D_8011EA30.scale[i]=(s16)(i+81);D_8011EA30.translate[i]=(s16)(i-41);}
}
static unsigned count;
static void check(unsigned n,int idx,float h,float v,float w,float ht,float hc,float vc) {
    Descriptor72 expected[4],before[4];Viewport16 vp;
    float args[6],distance;u32 color;unsigned nc,nf;int ci;Descriptor72 *returned;
    void *first=&objects[(n+1)%8],*bounds=&objects[(n+5)%8];
    reset(n);historical_initialize(idx,first,bounds,h,v,w,ht,hc,vc);
    memcpy(expected,D_8017A510,sizeof expected);memcpy(before,pre_call,sizeof before);
    memcpy(args,call_args,sizeof args);vp=D_8011EA30;distance=D_80151AA0;color=D_80124FC8;
    nc=calls;nf=fill_calls;ci=call_index;
    reset(n);returned=arb_rate_set(idx,first,bounds,h,v,w,ht,hc,vc);
    if(returned!=&D_8017A510[idx] || memcmp(expected,D_8017A510,sizeof expected) ||
       memcmp(before,pre_call,sizeof before) || memcmp(args,call_args,sizeof args) ||
       memcmp(&vp,&D_8011EA30,sizeof vp) || distance!=D_80151AA0 || color!=D_80124FC8 ||
       nc!=calls || nf!=fill_calls || ci!=call_index || calls!=1 || fill_calls!=1) fail();
    count++;
}
int main(void) {
    static const float boundary[]={-16384.0f,-16383.75f,-1.0f,-0.0f,0.0f,0.25f,1.0f,16383.0f,16383.5f};
    unsigned i,j,k,n,r,g,b,packed_count=0;volatile u32 checksum=0;
    for(i=0;i<4;i++)for(j=0;j<9;j++)for(k=0;k<9;k++)
        check(i*81+j*9+k,(int)i,90.0f,60.0f,boundary[j],240.0f,boundary[k],-boundary[j]*0.5f);
    for(n=0;n<20000;n++)check(n,(int)(n%4),(n%2)?90.0f:0.0f,(n%3)?60.0f:0.0f,
        (float)(n%16000)+0.25f,(float)(n%12000)+1.0f,(float)((int)(n%32000)-16000)*0.5f,(float)((int)(n%21000)-10000)*0.5f);
    for(r=0;r<256;r++)for(g=0;g<256;g++)for(b=0;b<256;b++) {
        u16 old=(u16)GPACK_RGBA5551(r,g,b,1);
        u16 revised=(u16)(((u16)(((r<<8)&0xf800)|((g<<3)&0x7c0)|((b>>2)&0x3e)))|1);
        if(old!=revised)fail();checksum^=old;packed_count++;
    }
    printf("%u finite state/call/return cases; %u exhaustive RGB-byte packing cases; valid signed-short boundaries; UBSan clean; checksum=%u",count,packed_count,(unsigned)checksum);
    return 0;
}
