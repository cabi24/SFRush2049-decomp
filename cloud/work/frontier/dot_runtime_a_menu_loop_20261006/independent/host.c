#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#ifndef REVIEW_SOURCE
#define REVIEW_SOURCE "../candidate.c"
#endif
#include REVIEW_SOURCE
s16 D_8014A108;
s32 D_8014A110;
s8 D_803BA028[32768];
volatile u8 D_80140BDC;
char D_803B85E8[1];
MenuData D_8017A4E0;
static Texture texture;
static MenuIndices indices[2];
static char tokens[2][65539];
static char *texts[2][65539];
uint32_t trace[20][7];
int count;
static int mutate_at,mutation,font_height,initialized;
static uint32_t token(void *p) {
    int i;uintptr_t value=(uintptr_t)p;
    for(i=0;i<2;i++) if(value>=(uintptr_t)&tokens[i][0] && value<=(uintptr_t)&tokens[i][65538]) return 0x80500000+i*0x100000+(value-(uintptr_t)&tokens[i][0])*4;
    abort();return 0;
}
static void emit(uint32_t kind,uint32_t a,uint32_t b,uint32_t c,uint32_t d,uint32_t e,uint32_t f) {
    uint32_t *p=trace[count++];
    if(count>20) abort();
    p[0]=kind;p[1]=a;p[2]=b;p[3]=c;p[4]=d;p[5]=e;p[6]=f;
    if(count==mutate_at && kind!=5) {
        switch(mutation) {
            case 0: D_8014A108=0; break;
            case 1: D_8014A108=2; break;
            case 2: D_8014A108=1; break;
            case 3: D_8014A110=2; break;
            case 4: D_8014A110=0; break;
            case 5: D_803BA028[0]=1; break;
            case 6: D_803BA028[0]=0; break;
            case 7: D_80140BDC=(u8)(D_80140BDC+129); break;
            case 8: texture.height=(u16)(texture.height+32769); break;
            case 9: font_height=font_height==637?-128:637; break;
            case 10: D_8017A4E0.indices=&indices[1]; break;
            case 11: D_8017A4E0.text=texts[1]; break;
            case 12: D_8017A4E0.indices=&indices[1];D_8017A4E0.text=texts[1]; break;
            case 13: D_8014A108=-32768; break;
            default: abort();
        }
    }
}
void render_helper(f32 a) {union {f32 f;uint32_t u;} v;v.f=a;emit(1,v.u,0,0,0,0,0);}
void *object_create(s32 a) {emit(2,a,0,0,0,0,0);return &tokens[0][0];}
void dispatch_handler(s32 a) {emit(3,a,0,0,0,0,0);}
Texture *func_800B24EC(char *name,s16 *out,s8 lo,s8 hi,s32 err) {
    if(name!=D_803B85E8 || !out) abort();
    *out=(s16)0xA55A;
    emit(4,0x803B85E8,0x807EFFF6,lo,hi,err,0);
    return &texture;
}
s16 object_bytes_sum_global(void) {int ret=font_height;emit(5,0,0,0,0,0,0);return (s16)ret;}
void state_utility(s16 x,s16 y,void *p) {emit(6,x,y,token(p),0,0,0);}
int run(int players,int mode,int skip,int tables,int height,int font,int first,int hook,int kind) {
    int i,j;
    if(!initialized) {
        for(i=0;i<2;i++) for(j=0;j<65539;j++) texts[i][j]=&tokens[i][j];
        initialized=1;
    }
    memset(D_803BA028,(s8)skip,sizeof(D_803BA028));
    D_8014A108=(s16)players;D_8014A110=mode;D_80140BDC=(u8)tables;
    texture.height=(u16)height; font_height=font;
    indices[0].first=(u16)first;indices[1].first=(u16)(first+32769);
    D_8017A4E0.indices=&indices[0];D_8017A4E0.text=texts[0];
    count=0;mutate_at=hook;mutation=kind;
    return func_803A4340((s32)0xF1E2D3C4);
}
