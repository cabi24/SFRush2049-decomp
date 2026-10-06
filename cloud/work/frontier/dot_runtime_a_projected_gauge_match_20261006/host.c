/* Contract hooks and unchanged-source host execution. */
#ifndef CANDIDATE
#define CANDIDATE "candidate.c"
#endif
#include CANDIDATE
#include <string.h>
#include <stdlib.h>
s16 D_8014A108;
s8 D_803BA028[4],D_803B9FD0[4];
MenuSlot D_803AF9A8[4][44];
float D_803B97A8,D_803B97AC,D_803B97B0,D_803B97B4,D_803B97B8;
float D_80111310[4][13],D_80111414[4][13];
ProjectContext D_80150B70[4];
static int *output,n,p,k,px,py,hook,updates;
static Blit *owner;
static float fvalue(unsigned x) {float f;memcpy(&f,&x,4);return f;}
static void put(int x) {if(n>=256)abort();output[n++]=x;}
static s32 callback(Blit *b) {(void)b;return 1;}
static void effect(int stage) {
 if(hook==stage) {
  owner->Width=77;owner->Height=-13;owner->AnimID^=0xffff00ffU;
  if(p<4){D_803AF9A8[p][k].state.alpha=199;D_803B9FD0[p]=2;}
 }
}
void Input_ApplyPadConfig(Blit *b) {
 if(b!=owner)abort();put(3);updates++;if(updates==1)effect(1);
}
s8 input_new_data_wrapper(Blit *b,s32 hide) {
 if(b!=owner)abort();put(1);put(hide);
 if(b->Hide!=hide){b->Hide=hide;Input_ApplyPadConfig(b);}
 return b->Hide;
}
void brake_light_update(s32 player,float *v,ProjectContext *ctx,void *ignored,s16 *out) {
 if(player!=p || v!=&D_803AF9A8[p][k+19].matrix[12] || ctx!=&D_80150B70[p] || ignored)abort();
 put(2);put(player);put((k+19)*64+48);out[0]=px;out[1]=py;effect(2);
}
int host_run(const int *c,int *result) {
 Blit b;int i,j,ret;
 output=result;n=0;p=c[0];k=c[1];px=c[11];py=c[12];hook=c[13];updates=0;owner=&b;
 memset(&b,0,sizeof b);memset(D_803AF9A8,0,sizeof D_803AF9A8);
 D_8014A108=c[3];D_803B97A8=-1.0f;D_803B97AC=1.0f;D_803B97B0=2.0f;D_803B97B4=.25f;D_803B97B8=.5f;
 for(i=0;i<4;i++) {
  D_803BA028[i]=i==p?c[4]:0;D_803B9FD0[i]=i==p?c[10]:0;
  for(j=0;j<13;j++){D_80111310[i][j]=.75f+(float)(i+j)/4;D_80111414[i][j]=.75f+(float)(i+j)/8;}
 }
 if(p<4){D_803AF9A8[p][k].state.depth=fvalue(c[8]);D_803AF9A8[p][k].state.alpha=c[9];}
 b.AnimID=((u32)k<<16)|((u32)p<<4)|(u32)c[2];b.AnimFunc=callback;b.Hide=c[5];b.Width=c[6];b.Height=c[7];
 b.X=-321;b.Y=654;b.Left=-17;b.Right=-18;b.Top=-19;b.Bot=-20;b.Alpha=77;
 ret=func_803A3DA4(&b);
 put(4);put(ret);put(b.X);put(b.Y);put(b.Left);put(b.Right);put(b.Top);put(b.Bot);put(b.Alpha);put(b.Hide);put(b.AnimFunc!=0);put((s32)b.AnimID);put(b.Width);put(b.Height);
 return n;
}
