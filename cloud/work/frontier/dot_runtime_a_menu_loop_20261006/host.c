/* The unchanged candidate compiled under bounded, side-effecting O32 contracts. */
#include <string.h>
#include <stdlib.h>
#include "candidate.c"
s16 D_8014A108;
s32 D_8014A110;
s8 D_803BA028[4];
volatile u8 D_80140BDC;
char D_803B85E8[1];
MenuData D_8017A4E0;
static Texture textures[2];
static MenuIndices indices[2];
static char strings[2][12][1];
static char *texts[2][12];
static int rows[32][12], calls, mutation, new_count, font_height, bank;
static void emit(int kind,int a,int b,int c,int d,int e) {
    int i;int *r;
    if(calls>=32)abort();
    r=rows[calls++];r[0]=kind;r[1]=a;r[2]=b;r[3]=c;r[4]=d;r[5]=e;
    r[6]=D_8014A108;r[7]=D_8014A110;r[8]=D_80140BDC;
    r[9]=bank;r[10]=D_803BA028[0];r[11]=indices[bank].first;
    /* Renderer-cache writes are outside this harness. Menu storage stays
     * disjoint from the font helper's writes under the tested contract. */
    if(calls==mutation && kind!=5) {
        D_8014A108=(s16)new_count;
        D_8014A110=D_8014A110==2 ? 0 : 2;
        D_80140BDC=(u8)(255-D_80140BDC);
        for(i=0;i<4;i++)D_803BA028[i]=D_803BA028[i]==1 ? 0 : 1;
        bank=1;D_8017A4E0.indices=&indices[bank];D_8017A4E0.text=texts[bank];
    }
}
static int token(void *p) {
    int i,j;
    for(i=0;i<2;i++)for(j=0;j<12;j++)if(p==strings[i][j])return 100+16*i+j;
    abort();return 0;
}
void render_helper(f32 v) {emit(1,(int)v,0,0,0,0);}
void *object_create(s32 n) {emit(2,n,0,0,0,0);return 0;}
void dispatch_handler(s32 n) {emit(3,n,0,0,0,0);}
Texture *func_800B24EC(char *name,s16 *out,s8 lo,s8 hi,s32 err) {
    if(name!=D_803B85E8 || out==0)abort();
    *out=1234;emit(4,9000,lo,hi,err,1234);return &textures[bank];
}
s16 object_bytes_sum_global(void) {emit(5,font_height,0,0,0,0);return (s16)font_height;}
void state_utility(s16 x,s16 y,void *p) {emit(6,x,y,token(p),0,0);}
int host_run(int count,int flag,int mode,int texture_height,int font,int tables,int hook,int replacement,int *out) {
    int i,j;
    memset(rows,0,sizeof(rows));calls=0;mutation=hook;new_count=replacement;font_height=font;bank=0;
    D_8014A108=(s16)count;D_8014A110=mode;D_80140BDC=(u8)tables;
    for(i=0;i<4;i++)D_803BA028[i]=(s8)flag;
    for(i=0;i<2;i++) {
        memset(&textures[i],0xA5,sizeof(textures[i]));textures[i].height=(u16)(texture_height^(i?65535:0));
        indices[i].first=(u16)(2+i);
        for(j=0;j<12;j++)texts[i][j]=strings[i][j];
    }
    D_8017A4E0.indices=&indices[0];D_8017A4E0.text=texts[0];
    if(func_803A4340(0x76543210)!=1)abort();
    memcpy(out,rows,sizeof(rows));return calls;
}
