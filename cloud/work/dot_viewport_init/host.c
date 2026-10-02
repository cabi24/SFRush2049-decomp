/* Portable host semantic harness. The callback observes and mutates real globals. */
#include <assert.h>
#include <stdint.h>
#include <string.h>
#include <stddef.h>
#include "candidate.c"
struct Viewport { unsigned char opaque[16]; };
int D_8002AFC0, D_8002AFC4;
unsigned char D_80146204, D_8017A63C;
Viewport D_8011EA30;
Bounds D_80149870;
ViewBinding D_8017A510;
void *D_80154188;
static int next_width, next_height;
static uint32_t capture[9];
static unsigned calls;
static uint32_t bits(float f) { uint32_t u; memcpy(&u,&f,4); return u; }
void arb_rate_set(int index, Viewport *v, Bounds *b, void *context,
                  float zero, float width, float height, float half_width, float half_height)
{
    assert(index==0 && v==&D_8011EA30 && b==&D_80149870 && context==D_80154188);
    assert(D_80146204==1 && D_8017A63C==0);
    calls++;
    capture[0]=(uint32_t)index; capture[1]=0x8011EA30; capture[2]=0x80149870;
    capture[3]=0x12345678; capture[4]=bits(zero); capture[5]=bits(width);
    capture[6]=bits(height); capture[7]=bits(half_width); capture[8]=bits(half_height);
    D_8002AFC0=next_width; D_8002AFC4=next_height;
    memset(&D_80149870,0xA5,sizeof(D_80149870));
}
void run(int width,int height,int after_width,int after_height,uint32_t *out)
{
    D_8002AFC0=width;D_8002AFC4=height;next_width=after_width;next_height=after_height;
    D_80146204=0xA5;D_8017A63C=0x5A;D_80154188=&D_80154188;calls=0;
    memset(&D_80149870,0xCC,sizeof(D_80149870));
    memset(&D_8017A510,0,sizeof(D_8017A510));
    memset(&D_8011EA30,0x37,sizeof(D_8011EA30));
    func_800A5A40();
    assert(calls==1 && D_8017A510.viewport==&D_8011EA30 && D_8017A510.bounds==&D_80149870);
    assert(D_80146204==1 && D_8017A63C==0);
    assert(D_80149870.left==0 && D_80149870.top==0);
    assert(D_80149870.right==(unsigned short)after_width && D_80149870.bottom==(unsigned short)after_height);
    assert(D_8002AFC0==after_width && D_8002AFC4==after_height);
    { unsigned i; for(i=0;i<sizeof(D_8011EA30.opaque);i++) assert(D_8011EA30.opaque[i]==0x37); }
    assert(D_80154188==&D_80154188);
    memcpy(out,capture,sizeof(capture));
    out[9]=D_80149870.right;out[10]=D_80149870.bottom;
}
#ifdef HOST_MAIN
static uint32_t seed=0xA5A40;
static uint32_t rnd(void) { seed^=seed<<13;seed^=seed>>17;seed^=seed<<5;return seed; }
int main(void)
{
    unsigned i;uint32_t out[11];int w,h,aw,ah;
    assert(sizeof(int)==4 && sizeof(float)==4 && sizeof(Bounds)==8 && offsetof(Bounds,bottom)==6);
    for(i=0;i<100000;i++) {
        uint32_t u=rnd();memcpy(&w,&u,4);u=rnd();memcpy(&h,&u,4);
        u=rnd();memcpy(&aw,&u,4);u=rnd();memcpy(&ah,&u,4);
        run(w,h,aw,ah,out);
        assert(out[4]==bits(0.0f) && out[5]==bits((float)w) && out[6]==bits((float)h));
        assert(out[7]==bits((float)(w/2)) && out[8]==bits((float)(h/2)));
    }
    return 0;
}
#endif
