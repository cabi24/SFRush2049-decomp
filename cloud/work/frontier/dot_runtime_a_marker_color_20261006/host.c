/* Unchanged source executed with contract hooks. No renderer implementation. */
#ifndef CANDIDATE
#define CANDIDATE "../../../matches/ovl_a/func_803A3A6C.c"
#endif
#include CANDIDATE
#include <string.h>
#include <stdlib.h>
s16 D_8014A108;
s8 D_803BA028[4],D_803B9FD0[4];
MenuSlot D_803AF9A8[4][44];
float D_803B97A0,D_803B97A4;
ProjectContext D_80150B70[4];
s8 D_80111611[4][13],D_80111655[4][13],D_80111699[4][13];
Color D_803B9B60[4][3],D_801226C0[16];
static int *out,n,player,marker,hook,updates,px,py;
static Blit *active;
static u32 bits(float v) { u32 x; memcpy(&x,&v,4); return x; }
static float value(u32 x) { float v;memcpy(&v,&x,4);return v; }
static void put(int v) { if(n>=2048) abort();out[n++]=v; }
static s32 callback(Blit *p) {(void)p;return 1;}
static void emit(int event,int arg,Blit *b) {
    int i,j,k,image=-1;
    for(i=0;i<4;i++)for(j=0;j<3;j++)if(b->Image==&D_803B9B60[i][j])image=(i*3+j)*4;
    put(event);put(arg);put(b->X);put(b->Y);put(b->Width);put(b->Height);
    put(b->Alpha);put(b->Hide);put(b->AnimFunc!=0);put((s32)b->AnimID);put(image);put(D_8014A108);
    put(player<4?D_803AF9A8[player][marker].state.alpha:-1);put(player<4?D_803B9FD0[player]:-1);
    for(i=0;i<4;i++)for(j=0;j<3;j++){u8 *a=(u8*)&D_803B9B60[i][j];for(k=0;k<4;k++)put(a[k]);}
}
static void effect(Blit *b,int stage) {
    if((hook==1 && stage==1)||(hook==2 && stage==2)) {
        b->Width=-31;b->Height=19;b->AnimID^=0xa5551230U;
        if(player<4){D_803AF9A8[player][marker].state.alpha=201;D_803B9FD0[player]=2;}
    }
    if(stage==1 && hook==3)b->Hide=0;
    if(stage==1 && hook==4)b->Hide=-1;
}
void Input_ApplyPadConfig(Blit *b) {emit(2,0,b);updates++;if(updates==1)effect(b,1);}
s8 input_new_data_wrapper(Blit *b,s32 hide) {
    emit(1,hide,b);
    if(hide!=b->Hide) {b->Hide=hide;Input_ApplyPadConfig(b);}
    return b->Hide;
}
void brake_light_update(s32 p,float *v,ProjectContext *ctx,void *vout,s16 *sout) {
    int i;if(p!=player || v!=&D_803AF9A8[player][marker+19].matrix[12] || ctx!=&D_80150B70[player] || vout)abort();
    put(5);put(p);put((marker+19)*64+48+player*2816);put(player*152);put(vout!=0);
    for(i=0;i<3;i++)put((s32)bits(v[i]));
    for(i=0;i<3;i++)put((s32)bits(ctx->position[i]));
    emit(3,0,active);sout[0]=px;sout[1]=py;effect(active,2);
}
int host_run(const int *c,int *result) {
    Blit b; int i,j,k,ret;u32 id;Color before_colors[4][3];
    u8 copy_slots[sizeof(D_803AF9A8)],copy_ctx[sizeof(D_80150B70)];
    u8 copy_palette[sizeof(D_801226C0)];
    out=result;n=0;player=c[0];marker=c[1];hook=c[14];updates=0;px=c[12];py=c[13];active=&b;
    memset(&b,0x5a,sizeof(b));D_8014A108=c[2];D_803B97A0=value(c[9]);D_803B97A4=value(c[10]);
    for(i=0;i<4;i++) {
        D_803BA028[i]=(i==player)?c[3]:i+3;D_803B9FD0[i]=(i==player)?c[11]:(i+3)%13;
        for(j=0;j<44;j++) {for(k=0;k<16;k++)D_803AF9A8[i][j].matrix[k]=(float)(i*1000+j*20+k);D_803AF9A8[i][j].state.alpha=(u8)(i*41+j*7+1);}
        for(j=0;j<3;j++){for(k=0;k<3;k++)D_80150B70[i].rotation[j][k]=(float)(i*20+j*3+k+1);D_80150B70[i].position[j]=(float)(i*20+j+10);}
        for(j=0;j<13;j++) {D_80111611[i][j]=(i*3+j)%16;D_80111655[i][j]=(i*5+j+5)%16;D_80111699[i][j]=(i*7+j+10)%16;}
        for(j=0;j<3;j++){u8 *a=(u8*)&D_803B9B60[i][j];for(k=0;k<4;k++)a[k]=(u8)(i*61+j*19+k+17);}
    }
    for(i=0;i<16;i++){D_801226C0[i].r=i*13+1;D_801226C0[i].g=i*7+2;D_801226C0[i].b=i*3+3;D_801226C0[i].a=i*5+4;}
    if(player<4){D_803AF9A8[player][marker].state.depth=value(c[7]);D_803AF9A8[player][marker].state.alpha=c[8];}
    b.X=-321;b.Y=654;b.Width=c[5];b.Height=c[6];b.Alpha=77;b.Hide=c[4];b.AnimFunc=callback;b.Image=0;
    id=((u32)marker<<16)|(u32)player|((u32)c[15]&0xff00fff0U);b.AnimID=id;
    memcpy(copy_slots,D_803AF9A8,sizeof copy_slots);memcpy(copy_ctx,D_80150B70,sizeof copy_ctx);memcpy(copy_palette,D_801226C0,sizeof copy_palette);memcpy(before_colors,D_803B9B60,sizeof before_colors);
    ret=func_803A3A6C(&b);emit(4,ret,&b);
    if(player<4) copy_slots[player*2816+marker*64+60]=D_803AF9A8[player][marker].state.alpha;
    if(memcmp(copy_slots,D_803AF9A8,sizeof copy_slots)||memcmp(copy_ctx,D_80150B70,sizeof copy_ctx)||memcmp(copy_palette,D_801226C0,sizeof copy_palette))abort();
    return n;
}
