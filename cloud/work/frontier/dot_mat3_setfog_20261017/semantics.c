/* Full-width bounded field/return and packed-color tests. */
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "candidate.c"
#include "historical_body.inc"

Descriptor72 D_8017A510[4];
u32 D_80124FC8;
static unsigned fill_calls;
static int objects[8];
/* The native helper is separately byte-verified. Unsigned arithmetic here
 * models its 32-bit machine shift without the historical signed-promotion UB. */
void func_800A5560(u16 color) { D_80124FC8=((u32)color<<16)|(u32)color;fill_calls++; }
static void fail(void) { fputs("setfog differential mismatch\n",stderr);exit(3); }
static unsigned count;
static void reset(void) {
    unsigned i;
    memset(D_8017A510,0,sizeof D_8017A510);D_80124FC8=0x12345678u;fill_calls=0;
    for(i=0;i<4;i++){
        D_8017A510[i].first=&objects[i];D_8017A510[i].bounds=&objects[i+4];
        D_8017A510[i].width=17.0f+(float)i;D_8017A510[i].height=19.0f+(float)i;
        D_8017A510[i].zoom=2.5f;D_8017A510[i].fog=2000.0f;
        D_8017A510[i].range_start=-123;D_8017A510[i].range_end=456;
        D_8017A510[i].red=1;D_8017A510[i].green=2;D_8017A510[i].blue=3;D_8017A510[i].alpha=4;
    }
}
static void check(unsigned idx,s16 min,s16 max,u8 red,u8 green,u8 blue,u8 alpha) {
    Descriptor72 expected[4],*p;u32 color;unsigned nc;
    u16 packed=(u16)(((red>>3)<<11)|((green>>3)<<6)|((blue>>3)<<1)|1);
    reset();historical_setfog(min,max,red,green,blue,alpha,(s32)idx);
    memcpy(expected,D_8017A510,sizeof expected);color=D_80124FC8;nc=fill_calls;
    reset();p=func_800A7480(min,max,red,green,blue,alpha,idx);
    if(p!=&D_8017A510[idx] || memcmp(expected,D_8017A510,sizeof expected) || color!=D_80124FC8 || nc!=fill_calls || fill_calls!=1 ||
       p->range_start!=min || p->range_end!=(u16)max || p->red!=red || p->green!=green || p->blue!=blue || p->alpha!=alpha ||
       D_80124FC8!=(((u32)packed<<16)|packed))fail();
    count++;
}
int main(void) {
    int v;unsigned i,n,r,g,b,packed_count=0;volatile u32 checksum=0;
    for(i=0;i<4;i++)for(v=-32768;v<=32767;v++) {
        n=(unsigned)(v+32768);
        check(i,(s16)v,(s16)(-1-v),(u8)n,(u8)(n>>8),(u8)(n*13u),(u8)(n*17u));
    }
    for(r=0;r<256;r++)for(g=0;g<256;g++)for(b=0;b<256;b++) {
        u16 old=(u16)GPACK_RGBA5551(r,g,b,1);
        u16 revised=(u16)((u16)(((r<<8)&0xf800)|((g<<3)&0x7c0)|((b>>2)&0x3e))|1);
        u16 expected=(u16)(((r>>3)<<11)|((g>>3)<<6)|((b>>3)<<1)|1);
        if(old!=revised || revised!=expected)fail();checksum^=revised;packed_count++;
    }
    printf("%u state/return cases across four windows and every signed-short value; %u exhaustive RGB triples; unsigned native-shift helper model; UBSan clean; checksum=%u",count,packed_count,(unsigned)checksum);
    return 0;
}
