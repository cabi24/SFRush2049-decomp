/* Unchanged candidate and accepted rear-camera body, with bounded external-call models. */
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <math.h>
#include <assert.h>
typedef signed char s8;
typedef unsigned char u8;
typedef short s16;
typedef int s32;
typedef float f32;
float sqrtf(float);
float fabsf(float);
typedef struct V3 { float x,y,z; } V3;
typedef struct Fx { u8 p0[0x60]; V3 n60; u8 p6c[0x18]; float v84[3]; u8 p90[8]; } Fx;
typedef struct HN { u8 p[0x2c]; s32 f2c; } HN;
typedef struct Q { u8 p[0x48]; HN **h; } Q;
typedef struct Cr {
 u8 p0[8];f32 pos[3];f32 vx,vy,vz;u8 p20[0xc];f32 m[9];u8 p50[0x58];
 f32 f_a8,f_ac;u8 pb0[0x46];s16 s_f6;u8 pf8[0x264];s8 player,mode,mode2;
 u8 p35f[0x21];Q *q;
} Cr;
typedef char CheckV3[(sizeof(V3)==12)?1:-1];
typedef char CheckFx[(sizeof(Fx)==152)?1:-1];
typedef char CheckCar[(offsetof(Cr,q)==0x380 && offsetof(Cr,player)==0x35c)?1:-1];
V3 D_8012E690[4],D_801526A8[4];Fx D_80150B70[4];
s16 D_8012E6C8[4];f32 D_8012E6E8[4],D_801526F8[4],D_801526E0[4],D_80152720[4];
f32 D_801108C8[4],D_8002EB94;s32 state_word_a;
static uint32_t events[128];static int nevents,selected,slot;
static uint32_t fb(float f){uint32_t x;memcpy(&x,&f,4);return x;}
static float fv(uint32_t x){float f;memcpy(&f,&x,4);return f;}
static void event(int n,float *a,float *b){int i;events[nevents++]=n;for(i=0;i<3;i++)events[nevents++]=a?fb(a[i]):0;for(i=0;i<3;i++)events[nevents++]=b?fb(b[i]):0;}
void vector_normalize_length(float *a,V3 *b){event(1,a,0);b->x=a[0];b->y=a[1];b->z=a[2];}
void func_800E9234(Cr *c){assert(c->player==slot);event(2,0,0);}
s32 func_800CDE38(HN **h){assert(h[0]->f2c);event(3,0,0);return selected;}
void func_800E8D50(Cr *c,float *p,s32 uvs,float *r){int i;assert(c->player==slot && uvs==0x2468);event(4,p,r);for(i=0;i<3;i++)D_80150B70[slot].v84[i]=p[i]+r[i];}
void func_8009E820(float *a,float *b,float *m){int i;for(i=0;i<3;i++)b[i]=(a[0]*m[i]+a[1]*m[3+i])+a[2]*m[6+i];}
void func_800A61B0(float *a,float *b,float *m){int i;for(i=0;i<3;i++)b[i]=(a[0]*m[3*i]+a[1]*m[3*i+1])+a[2]*m[3*i+2];}
void func_800CFDEC(float *a,float *b,s16 n,float lo,float hi,float t,float *out){float k;int i;k=hi-lo;if(hi!=lo)k=(t-lo)/k;for(i=0;i<n;i++)out[i]=(b[i]-a[i])*k+a[i];}
#define MAX_VEL 100.0f
#include "rear_camera.inc"
#include "candidate.c"
void run_case(const uint32_t *in,uint32_t *out){
 Cr car,before;Q q;HN h,*hp=&h;float pos[3];int i,j,k=0;uint32_t *old;
 memset(&car,0xa5,sizeof(car));memset(&q,0,sizeof(q));memset(&h,0,sizeof(h));
 slot=in[1];selected=(int)in[5];nevents=0;car.player=slot;car.mode=(s8)in[6];car.mode2=7;car.q=&q;q.h=&hp;h.f2c=in[4];
 for(i=0;i<3;i++){car.pos[i]=fv(in[11+i]);pos[i]=fv(in[14+i]);}
 car.vx=fv(in[20]);car.vy=fv(in[21]);car.vz=fv(in[22]);car.f_a8=fv(in[23]);car.f_ac=fv(in[24]);
 for(i=0;i<9;i++)car.m[i]=fv(in[25+i]);
 state_word_a=in[3];D_8002EB94=fv(in[8]);D_801108C8[0]=fv(in[9]);D_801108C8[1]=fv(in[10]);D_801108C8[2]=2.0f;D_801108C8[3]=3.0f;
 for(i=0;i<4;i++){
  D_8012E690[i].x=100+i;D_8012E690[i].y=200+i;D_8012E690[i].z=300+i;
  D_801526A8[i].x=400+i;D_801526A8[i].y=500+i;D_801526A8[i].z=600+i;
  memset(&D_80150B70[i],0xa5,sizeof(Fx));
  D_80150B70[i].v84[0]=700+i;D_80150B70[i].v84[1]=800+i;D_80150B70[i].v84[2]=900+i;
  D_8012E6C8[i]=3;D_8012E6E8[i]=10+i;D_801526F8[i]=20+i;D_801526E0[i]=12+i;D_80152720[i]=.25f;
 }
 D_8012E690[slot].x=fv(in[17]);D_8012E690[slot].y=fv(in[18]);D_8012E690[slot].z=fv(in[19]);
 D_8012E6C8[slot]=(s16)in[2];D_8012E6E8[slot]=fv(in[7]);D_801526F8[slot]=fv(in[34]);D_801526E0[slot]=fv(in[35]);
 before=car;func_800E95DC((s16)in[0],&car,pos,0x2468);
 before.mode=car.mode;before.mode2=car.mode2;assert(memcmp(&before,&car,sizeof(car))==0);
 out[k++]=(uint32_t)(s32)car.mode;out[k++]=(uint32_t)(s32)car.mode2;
 for(i=0;i<3;i++)out[k++]=fb(pos[i]);
 for(i=0;i<4;i++){
  out[k++]=(uint32_t)(s32)D_8012E6C8[i];out[k++]=fb(D_8012E6E8[i]);out[k++]=fb(D_80152720[i]);
  out[k++]=fb(D_8012E690[i].x);out[k++]=fb(D_8012E690[i].y);out[k++]=fb(D_8012E690[i].z);
  out[k++]=fb(D_801526A8[i].x);out[k++]=fb(D_801526A8[i].y);out[k++]=fb(D_801526A8[i].z);
  for(j=0;j<3;j++)out[k++]=fb(D_80150B70[i].v84[j]);
  out[k++]=fb(D_80150B70[i].n60.x);out[k++]=fb(D_80150B70[i].n60.y);out[k++]=fb(D_80150B70[i].n60.z);
 }
 out[k++]=nevents;for(i=0;i<nevents;i++)out[k++]=events[i];while(k<128)out[k++]=0;
 (void)old;
}
