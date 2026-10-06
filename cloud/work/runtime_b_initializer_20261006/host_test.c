#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#ifndef CANDIDATE_PATH
#define CANDIDATE_PATH "nonmatch/func_803908D0.c"
#endif
#include CANDIDATE_PATH
volatile u8 D_80140BDC;
s16 D_80394F88;
u8 D_8039A520[24], D_80399B60[2496], D_8039A5F8[24], D_8039A538[192];
u8 D_80394F70[24], D_8039AE98[1440], D_8039AE80[24], D_8039A610[2160];
char *D_803942EC[10], *D_80394314[7], *D_80394330[5], *D_80394344[5];
s32 D_80399B18[10], D_80399B40[7];
s16 D_80399AF8[5], D_80399B08[5];
PlayerEffect D_80394F90[4];
static unsigned count, variant, calls, count_now;
static char tokens[64][2];
static s32 expected_models[17];
static s16 expected_textures[10];
static PlayerEffect original[4];
static unsigned expected_names[27];
static void fail(const char *s) { fprintf(stderr,"mismatch: %s at %u/%u call %u\n",s,count,variant,calls); exit(1); }
static void snapshot(void)
{
    unsigned i;
    if (D_80394F88 != 0 || D_80140BDC != count_now) fail("counter observation");
    for(i=0;i<17;i++) if((i<10?D_80399B18[i]:D_80399B40[i-10])!=expected_models[i]) fail("model state/order");
    for(i=0;i<10;i++) if((i<5?D_80399AF8[i]:D_80399B08[i-5])!=expected_textures[i]) fail("texture state/order");
    for(i=0;i<4;i++) {
        PlayerEffect expected = original[i];
        if(calls==31) expected.handle=-1;
        if(memcmp(&expected,&D_80394F90[i],sizeof(expected))) fail("complete player state");
    }
}
static void effect(unsigned j)
{
    unsigned k;
    if(variant&1) {
        count_now=(count_now+j*13+17)&255;
        D_80140BDC=(u8)count_now;
    }
    if(variant&2) {
        k=(j+3)%27;
        expected_names[k]=(expected_names[k]+11)%64;
        if(k<10) D_803942EC[k]=tokens[expected_names[k]];
        else if(k<17) D_80394314[k-10]=tokens[expected_names[k]];
        else if(k<22) D_80394330[k-17]=tokens[expected_names[k]];
        else D_80394344[k-22]=tokens[expected_names[k]];
    }
}
static unsigned handle(unsigned j)
{
    return count_now ? ((j*7+variant)&1023)|(((j+variant)%count_now)<<10) : 0;
}
void struct_fields_init(void *head, void *pool, s32 size, s32 n, u8 flags)
{
    static void *heads[4]={D_8039A520,D_8039A5F8,D_80394F70,D_8039AE80};
    static void *pools[4]={D_80399B60,D_8039A538,D_8039AE98,D_8039A610};
    static s32 sizes[4]={104,8,60,72};
    if(calls>=4 || head!=heads[calls] || pool!=pools[calls] || size!=sizes[calls] || n!=(calls==3?30:24) || flags) fail("pool arguments/order");
    snapshot();
    memset(head,(int)(0x31+calls),24);
    memset(pool,(int)(0x41+calls),(size_t)(size*n));
    calls++;
}
s32 string_copy_format(char *name, s8 first, s8 last, s8 complain)
{
    unsigned j, result;
    int high;
    if(calls<4 || calls>=21) fail("model call order");
    j=calls-4; high=(int)((count_now-1)&255); if(high>=128) high-=256;
    snapshot();
    if(name!=tokens[expected_names[j]] || first || last!=high || complain) fail("model arguments");
    result=count_now?handle(j):~0u;
    effect(j); expected_models[j]=(s32)result; calls++;
    return (s32)result;
}
NameEntry *func_800B24EC(char *name, s16 *out, s8 first, s8 last, s32 err)
{
    unsigned j, result;
    int high;
    if(calls<21 || calls>=31) fail("texture call order");
    j=calls-4; high=(int)((count_now-1)&255); if(high>=128) high-=256;
    snapshot();
    if(name!=tokens[expected_names[j]] || out!=(j<22?&D_80399AF8[j-17]:&D_80399B08[j-22]) || first || last!=high || err!=1) fail("texture arguments");
    result=handle(j); effect(j); *out=(s16)result; expected_textures[j-17]=(s16)result; calls++;
    return (NameEntry *)0;
}
void func_8038D1A8(void)
{
    unsigned i,j;
    u8 *heads[4]={D_8039A520,D_8039A5F8,D_80394F70,D_8039AE80};
    u8 *pools[4]={D_80399B60,D_8039A538,D_8039AE98,D_8039A610};
    unsigned sizes[4]={2496,192,1440,2160};
    if(calls!=31) fail("final helper order");
    snapshot();
    for(i=0;i<4;i++) {
        for(j=0;j<24;j++) if(heads[i][j]!=0x31+i) fail("pool header preservation");
        for(j=0;j<sizes[i];j++) if(pools[i][j]!=0x41+i) fail("pool storage preservation");
    }
    calls++;
}
int main(void)
{
    unsigned i;
    for(count=0;count<256;count++) for(variant=0;variant<4;variant++) {
        calls=0;count_now=count;D_80140BDC=(u8)count;D_80394F88=123;
        for(i=0;i<27;i++) expected_names[i]=(i+count)%64;
        for(i=0;i<10;i++) D_803942EC[i]=tokens[expected_names[i]];
        for(i=0;i<7;i++) D_80394314[i]=tokens[expected_names[i+10]];
        for(i=0;i<5;i++) { D_80394330[i]=tokens[expected_names[i+17]]; D_80394344[i]=tokens[expected_names[i+22]]; }
        for(i=0;i<17;i++) { expected_models[i]=12345+(int)i; if(i<10) D_80399B18[i]=expected_models[i]; else D_80399B40[i-10]=expected_models[i]; }
        for(i=0;i<10;i++) { expected_textures[i]=(s16)(1000+i); if(i<5) D_80399AF8[i]=expected_textures[i]; else D_80399B08[i-5]=expected_textures[i]; }
        for(i=0;i<4;i++) { unsigned j; D_80394F90[i].handle=200+(int)i; for(j=0;j<12;j++) D_80394F90[i].transform[j]=(float)(count+i+j); original[i]=D_80394F90[i]; }
        func_803908D0();
        if(calls!=32) fail("final call count");
    }
    puts("1024 unchanged-source C89 boundary cases passed");return 0;
}
