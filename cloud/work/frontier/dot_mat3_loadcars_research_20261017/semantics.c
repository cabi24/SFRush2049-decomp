/* Finite differential test of the unmodified candidate body against an
 * independently structured model. Runtime service boundaries are controlled.
 * Only unused pointer fields in Vehicle2056 are replaced with u32 in the host
 * include, preserving target record geometry on a 64-bit host. */
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "host_candidate.inc"

Selection8 D_80153E88[13];
Vehicle2056 D_8014A250[6];
u32 D_8002E580[128],D_801174B4;
s16 D_8014A108,D_80142B08[13];
s8 D_80111604[32],D_80111648[32],D_8011168C[32];
struct Event { int kind; u32 a,b,c,d,e; };
struct Snapshot {
 Selection8 links[13];s16 handles[13];u32 seed,budget,resident,loads;
 int count;struct Event events[4096];
};
static struct Event events[4096];
static int count;static u32 seed,budget,resident,loads;
static unsigned coverage[8];
static void event(int k,u32 a,u32 b,u32 c,u32 d,u32 e) {
 if(count==4096){fputs("loadcars fixture exceeded service bound\n",stderr);exit(4);}
 events[count++]=(struct Event){k,a,b,c,d,e};
}
u32 audio_output_setup(Heap *p) {
 if(p){fputs("unexpected heap\n",stderr);exit(4);}event(1,budget,0,0,0,0);return budget;
}
f32 func_8008B2E4(f32 max) {
 u32 x;float result;seed=seed*UINT32_C(1103515245)+12345;
 x=(seed>>16)&32767;result=((float)x*max)/32768.0f;
 event(2,(u32)max,x,0,0,0);return result;
}
s32 func_80097694(s32 wave,s8 handle) {
 s32 r=((wave>=88&&wave<120)&&(resident&(UINT32_C(1)<<(wave-88))))?wave-88:-1;
 event(3,(u32)wave,(u32)(s32)handle,(u32)r,0,0);return r;
}
s32 audio_frame_sync(s32 wave,s32 again,s32 background,s32 handle,void *heap) {
 u32 size;event(4,(u32)wave,(u32)again,(u32)background,(u32)handle,(u32)(uintptr_t)heap);
 if(wave<0||wave>=128){fputs("invalid fixture wave\n",stderr);exit(4);}
 size=D_8002E580[wave];budget=budget>=size?budget-size:0;
 if(wave>=88&&wave<120)resident|=UINT32_C(1)<<(wave-88);
 return (s32)(0x7ffe + (loads++));
}
void func_800BB02C(s32 i,s32 kind,void *data) {
 int j;event(5,(u32)i,(u32)kind,(u32)(uintptr_t)data,0,0);
 /* Model only the documented link-payload and budget effects at this service
  * boundary; the native helper itself is separately required to match exactly. */
 if(budget<512) {
  for(j=0;j<i;j++)if(D_80153E88[j].model==kind){
   D_80153E88[i].type=D_80153E88[j].type;
   D_80153E88[i].tune=D_80153E88[j].tune;
   D_80153E88[i].variant=D_80153E88[j].variant;break;
  }
 } else budget-=512;
}
void func_8039156C(void){event(6,0,0,0,0,0);}

static void reference(int select) {
 int slot,loaded=0;
 if(select){func_8039156C();coverage[0]++;return;}
 for(slot=0;slot<13;slot++) {
  if(slot<6&&((D_801174B4&8)||D_8014A250[slot].mode==1)) {
   int previous,chosen,found,pass,attempt;
   coverage[1]++;
   do {
    chosen=(u8)func_8008B2E4(13.0f);
    for(previous=0;previous<slot;previous++)if(chosen==D_80153E88[previous].model)break;
   } while(previous<slot);
   D_80153E88[slot].model=(u8)chosen;
   D_80153E88[slot].type=D_80111604[chosen];
   D_80153E88[slot].tune=D_80111648[chosen];
   D_80153E88[slot].variant=D_8011168C[chosen];
   found=audio_output_setup(0)>=D_8002E580[chosen+88]+UINT32_C(512);
   if(!found) {
    for(pass=0;pass<2&&!found;pass++) {
     for(attempt=0;attempt<13;attempt++) {
      int available=pass||func_80097694(chosen+88,-1)<0;
      if(available&&audio_output_setup(0)>=D_8002E580[chosen+88]+UINT32_C(512)) {
       found=1;coverage[2+pass]++;break;
      }
      chosen=(chosen+1)%13;D_80153E88[slot].model=(u8)chosen;
     }
    }
   }
   if(found) {
    D_80142B08[slot]=(s16)audio_frame_sync(chosen+88,1,0,0,0);loaded++;
   } else {
    coverage[4]++;
    do{previous=(int)func_8008B2E4((float)loaded);}
    while(loaded>D_8014A108&&D_80153E88[previous].model>=13);
    D_80153E88[slot].model=D_80153E88[previous].model;
    D_80142B08[slot]=D_80142B08[previous];
   }
   func_800BB02C(slot,D_80153E88[slot].model,0);
  } else if(slot<6&&D_8014A250[slot].mode==2) {
   int chosen=D_80153E88[slot].model;
   int found=audio_output_setup(0)>=D_8002E580[chosen+88]+UINT32_C(512);
   if(!found)for(chosen=0;chosen<13;chosen++) {
    D_80153E88[slot].model=(u8)chosen;
    if(audio_output_setup(0)>=D_8002E580[chosen+88]+UINT32_C(512)){found=1;break;}
   }
   if(found) {
    coverage[5]++;D_80142B08[slot]=(s16)audio_frame_sync(chosen+88,1,0,0,0);
    func_800BB02C(slot,chosen,0);loaded++;
   } else {
    coverage[6]++;D_80153E88[slot].model=(u8)chosen;
    D_80153E88[slot].model=D_80153E88[0].model;
    D_80142B08[slot]=D_80142B08[0];func_800BB02C(slot,D_80153E88[slot].model,0);
   }
  } else {
   coverage[7]++;D_80153E88[slot].model=D_80153E88[0].model;
   D_80142B08[slot]=D_80142B08[0];
  }
 }
}
static void fixture(unsigned n) {
 static const u32 budgets[]={0,511,512,513,1023,2048,8192,1048576};
 static const u32 sizes[]={0,1,511,512,513,1024,0xfffffe00u,0xfffffe01u,0xffffffffu};
 unsigned i;memset(D_80153E88,0xa5,sizeof D_80153E88);memset(D_8014A250,0x5a,sizeof D_8014A250);
 for(i=0;i<13;i++){D_80153E88[i].model=(u8)((n+i*7)%19);D_80142B08[i]=(s16)(0x8000+i);}
 for(i=0;i<6;i++)D_8014A250[i].mode=(s8)((n>>(i%4))%4);
 for(i=0;i<32;i++){D_80111604[i]=(s8)(i*31+128);D_80111648[i]=(s8)(i*13+37);D_8011168C[i]=(s8)(i*7+255);}
 for(i=0;i<128;i++)D_8002E580[i]=sizes[(n+i*5)%9];
 D_801174B4=n&16?8:0;D_8014A108=6;
 seed=0x800bb140u^(n*0x9e3779b9u);budget=budgets[n%8];resident=n&32?0x1fffu:n*0x12345u;
 loads=0;count=0;memset(events,0,sizeof events);
 /* Directed rejection case: first loaded car is outside the random-car set,
  * second is valid, and the next human slot has no remaining load budget. */
 if(n>=4096&&n<4160) {
  D_801174B4=0;D_8014A108=0;
  for(i=0;i<6;i++)D_8014A250[i].mode=0;
  D_8014A250[0].mode=2;D_8014A250[1].mode=2;D_8014A250[2].mode=1;
  D_80153E88[0].model=15;D_80153E88[1].model=4;
  for(i=0;i<128;i++)D_8002E580[i]=1024;
  budget=3072;resident=0x1fff;
 }
 if(n>=4160) {
  D_801174B4=0;D_8014A108=6;budget=0;
  for(i=0;i<6;i++)D_8014A250[i].mode=2;
  for(i=0;i<128;i++)D_8002E580[i]=1024;
 }
}
static struct Snapshot snapshot(void) {
 struct Snapshot s;memset(&s,0,sizeof s);memcpy(s.links,D_80153E88,sizeof s.links);
 memcpy(s.handles,D_80142B08,sizeof s.handles);s.seed=seed;s.budget=budget;s.resident=resident;s.loads=loads;
 s.count=count;memcpy(s.events,events,sizeof events);return s;
}
int main(void) {
 unsigned n,i;struct Snapshot a,b;
 for(n=0;n<4224;n++) {
  int select=n<4096&&n%17==0?(n&1?-1:1):0;
  fixture(n);audio_interrupt_handler(select);a=snapshot();
  fixture(n);reference(select);b=snapshot();
  if(memcmp(&a,&b,sizeof a)){fputs("loadcars differential mismatch\n",stderr);return 3;}
 }
 for(i=0;i<8;i++)if(!coverage[i]){fputs("loadcars missing semantic branch\n",stderr);return 4;}
 printf("4224 finite candidate/model state-and-service-trace cases; all 8 branch classes covered\n");return 0;
}
