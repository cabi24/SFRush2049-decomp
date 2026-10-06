/* Independent, host-only callback mutation fixture. candidate.c is unmodified. */
#include <assert.h>
#include <stddef.h>
#include <stdint.h>
#include <string.h>
#include "candidate.c"
GearModel D_8014A250[4];
GearCar D_80152818[4];
GearPosition D_80115BE8[4][4];
GearAssets D_8017A4E0;
s16 D_80151AD0;
s8 D_80161394;
u8 D_801461D0[24];
static const char label0[]="A", label1[]="B", label2[]="C";
static const char *labels0[205],*labels1[205];
static const int *a;
static int *out,n,width_count;
static void mutate(void) {
 int i,j;
 if(n!=a[8]) return;
 switch(a[9]) {
 case 1: D_80151AD0=(s16)a[10]; break;
 case 2: labels0[204]=label1; break;
 case 3: D_8017A4E0.labels=labels1; break;
 case 4: D_8014A250[a[17]].gear=(s8)a[10]; break;
 case 5:
  for(i=0;i<4;i++) for(j=0;j<4;j++) {
   D_80115BE8[i][j].x=(s32)((u32)a[18]+43U*i+17U*j);
   D_80115BE8[i][j].y=(s16)((u32)a[19]+23U*i+11U*j);
  }
  break;
 case 6: D_8014A250[a[17]].hidden=(s8)a[10]; break;
 case 7: D_80152818[3-a[17]].kind=(s8)a[10]; break;
 case 8: D_80161394=(s8)a[10]; break;
 }
}
static void ev(int t,int x,int y,int z) {
 assert(n<120);out[4*n]=t;out[4*n+1]=x;out[4*n+2]=y;out[4*n+3]=z;n++;mutate();
}
static int label_id(const char *s) { return s==label0?1000:s==label1?1001:s==label2?1002:-1; }
void render_helper(float x) { assert(x==0.0f||x==-1.0f);ev(1,x==0.0f?0:-1,0,0); }
s32 osRecvMesg(void *q,void *m,s32 f) { assert(q==D_801461D0&&!m&&f==1);ev(2,1,0,0);return 41; }
s32 slot_state_setup(s32 s) { ev(3,s,0,0);return -0x1234; }
s32 osJamMesg(void *q,void *m,s32 f) { assert(q==D_801461D0&&!m&&!f);ev(4,0,0,0);return 42; }
void dispatch_handler(s32 color) { ev(5,color,0,0); }
u32 object_utility(const char *s,s32 limit) { int id=label_id(s);u32 w=(u32)a[6+(width_count++&1)];assert(id>=0&&limit==-1);ev(6,id,limit,(s32)w);return w; }
void state_utility(s16 x,s16 y,const char *s) { int id=label_id(s);if(id<0) { assert(s[1]==0);id=(u8)s[0]; }ev(7,x,y,id); }
int run_audit(const int *args,int *events) {
 int i,j,r;a=args;out=events;n=width_count=0;
 memset(D_8014A250,0,sizeof(D_8014A250));memset(D_80152818,0,sizeof(D_80152818));
 D_80151AD0=(s16)a[0];D_80161394=(s8)a[1];
 for(i=0;i<4;i++) {
  D_8014A250[i].gear=(s8)a[11+i];D_8014A250[i].car_index=(s16)(3-i);
  D_8014A250[i].hidden=(s8)((a[2]>>i&1)?a[15]:0);
  D_80152818[3-i].kind=(s8)((a[3]>>i&1)?a[16]:0);
  for(j=0;j<4;j++) {
   D_80115BE8[i][j].x=(s32)((u32)a[4]+43U*i+17U*j);
   D_80115BE8[i][j].y=(s16)((u32)a[5]+23U*i+11U*j);
  }
 }
 labels0[204]=label0;labels1[204]=label2;D_8017A4E0.labels=labels0;
 r=func_800EF288(0xFEDCBA98U);ev(8,r,D_80151AD0,0);return n;
}
typedef char AssertModelSize[sizeof(GearModel)==0x808?1:-1];
typedef char AssertGearOffset[offsetof(GearModel,gear)==0x730?1:-1];
typedef char AssertHiddenOffset[offsetof(GearModel,hidden)==10?1:-1];
typedef char AssertIndexOffset[offsetof(GearModel,car_index)==0x7C6?1:-1];
typedef char AssertCarSize[sizeof(GearCar)==0x3B8?1:-1];
typedef char AssertKindOffset[offsetof(GearCar,kind)==0xEF?1:-1];
typedef char AssertPositionSize[sizeof(GearPosition)==8?1:-1];
typedef char AssertY[offsetof(GearPosition,y)==6?1:-1];
#ifdef AUDIT_MAIN
int main(void) {
 int a[20]={4,1,0,0,32767,32767,-1,-2147483647,0,0,2,-1,0,3,127,1,1,0,2147483647,-32768};
 int events[512],i,j;unsigned int z=1;
 for(i=0;i<10000;i++) {
  for(j=4;j<=7;j++) {z=z*1664525U+1013904223U;a[j]=(int)z;}
  a[8]=1+i%14;a[9]=1+i%8;a[10]=1+i%4;a[17]=i%4;run_audit(a,events);
 }
 return 0;
}
#endif
