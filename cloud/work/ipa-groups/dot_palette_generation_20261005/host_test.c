/* Host contract test, not N64 execution. External services are checked stubs. */
#include <assert.h>
#include <limits.h>
#include <stdarg.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>
#define memcpy palette_memcpy
#define sprintf palette_sprintf
#include "palette.c"
#include "interpolate.c"
#undef memcpy
#undef sprintf

u8 D_8013FE90[4];
u32 D_8013F300[256];
Resource24 D_8013FEE0[4];
Handles64 D_80139320[4];
Model68 D_8012E700[8];
volatile u8 D_80140BDC;
char D_80123498[]="host-format";
s32 D_80152770;
static u16 original[256], temporary[258], destination[258];
static Resource24 source;
static int event, selected, expected_model;
static int interpolation_calls;
static u32 state=0x6f63a32bU;
static u32 random_word(void) { state=state*1664525U+1013904223U; return state; }

/* Explicit low-32-bit arithmetic and logical extraction implement the
 * target result independently of C's signed right-shift convention. */
static u32 expected_blend(s32 ratio,u32 left,u32 right)
{
    u32 masks[3]={0xf800,0x7c0,0x3e};
    u32 result=1, inverse=255U-(u32)ratio;
    int i;
    if(ratio==0) return left;
    if(ratio>=255) return right;
    for(i=0;i<3;i++) {
        u32 sum=(u32)((uint64_t)(left&masks[i])*inverse+
                      (uint64_t)(right&masks[i])*(u32)ratio);
        result|=(sum>>8)&masks[i];
    }
    return result;
}
void *audio_dma_sync(void *heap,u32 size)
{
    assert(event++==0 && heap==0 && size==512);
    assert(D_8013FE90[selected]==1);
    return &temporary[1];
}
s32 palette_sprintf(char *out,char *format,...)
{
    va_list args;
    assert(event++==1 && format==D_80123498);
    va_start(args,format);
    assert(va_arg(args,int)==expected_model+1);
    va_end(args);
    strcpy(out,"verified-name");
    return 13;
}
Resource24 *sound_bank_load(char *name,u16 *handle,s8 first,s8 last,s32 flag)
{
    assert(event++==2 && !strcmp(name,"verified-name"));
    assert(first==0 && last==(s8)(D_80140BDC-1) && flag==1);
    *handle=0xA5A5;
    return &source;
}
void *palette_memcpy(void *out,const void *in,u32 size)
{
    assert(size==512);
    if(event==3) assert(out==&temporary[1] && in==original);
    else { assert(event==4); assert(out==&destination[1] && in==&temporary[1]); }
    event++;
    return memcpy(out,in,size);
}
s32 osRecvMesg(void *queue,void **message,s32 flags)
{
    assert(event++==5 && queue==&D_80152770 && message==0 && flags==1);
    return 0;
}
void audio_reverb_update(u32 address,s32 tag)
{
    assert(event++==6 && address==(u32)(uintptr_t)&temporary[1] && tag==0);
}
s32 osJamMesg(void *queue,void *message,s32 flags)
{
    assert(event++==7 && queue==&D_80152770 && message==0 && flags==0);
    return 0;
}
static void palette_case(u8 first,u8 second,u8 third,int iteration)
{
    int i;
    u16 expected[256];
    u8 selectors[3];
    selected=iteration%4;
    expected_model=(iteration%31)-15;
    event=0;
    D_80140BDC=(u8)iteration;
    memset(D_8013FE90,0,sizeof(D_8013FE90));
    memset(D_8012E700,0,sizeof(D_8012E700));
    memset(D_80139320,0,sizeof(D_80139320));
    for(i=0;i<256;i++) {
        original[i]=(u16)random_word();
        expected[i]=original[i];
        D_8013F300[i]=random_word();
    }
    D_80139320[selected].root.halves.id=2;
    D_80139320[selected].secondary.halves.id=5;
    D_8013FEE0[selected].data=&destination[1];
    source.data=original;
    temporary[0]=destination[0]=0xCAF1;
    temporary[257]=destination[257]=0xD00D;
    if(first==0) {
        for(i=1;i<32;i++) {
            unsigned int value=(original[i]>>11)*10;
            if(value>255) value=255;
            expected[i]=(u16)(((value>>3)<<11)|((value>>3)<<6)|((value>>3)<<1)|1);
        }
    } else if(second==0) {
        for(i=33;i<64;i++) {
            unsigned int value=(original[i]>>11)*10;
            if(value>255) value=255;
            expected[i]=(u16)(((value>>3)<<11)|((value>>3)<<6)|((value>>3)<<1)|1);
        }
    }
    selectors[0]=first;selectors[1]=second;selectors[2]=third;
    for(i=0;i<3;i++) {
        int j, count=(i==2)?30:31;
        u32 color=D_8013F300[selectors[i]];
        expected[32+i*32]=(u16)color;
        for(j=0;j<count;j++) expected[33+i*32+j]=(u16)expected_blend(j*8,color,1);
    }
    sound_bank_unload((s16)selected,(s16)expected_model,first,second,third);
    assert(event==8);
    assert(!memcmp(&destination[1],expected,sizeof(expected)));
    assert(!memcmp(&temporary[1],expected,sizeof(expected)));
    assert(temporary[0]==0xCAF1 && temporary[257]==0xD00D);
    assert(destination[0]==0xCAF1 && destination[257]==0xD00D);
    for(i=0;i<4;i++) assert(D_8013FE90[i]==(i==selected));
    for(i=0;i<8;i++) assert(D_8012E700[i].palette==((i==2||i==5)?&D_8013FEE0[selected]:0));
}
int main(void)
{
    static const s32 ratios[]={INT_MIN,-65536,-256,-1,0,1,7,8,127,128,240,254,255,256,INT_MAX};
    unsigned int c,k;
    for(c=0;c<65536;c++) for(k=0;k<sizeof(ratios)/sizeof(ratios[0]);k++) {
        u32 right=(c*0x10001U)^0xCAFE4321U;
        assert(func_800B0EA0(ratios[k],c,right)==expected_blend(ratios[k],c,right));
        interpolation_calls++;
    }
    for(c=0;c<4096;c++) palette_case((u8)(c%256),(u8)((c/2)%256),(u8)((c/3)%256),c);
    /* Explicitly cover the alternate brightening branch and zero colors. */
    palette_case(0,0,0,4096);
    palette_case(1,0,255,4097);
    printf("PASS: %d interpolation cases; 4098 palette cases\n",interpolation_calls);
    return 0;
}
