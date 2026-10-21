/* flags: -g0 -O3 -mips2 -G 0 -non_shared -Wab,-r4300_mul */
typedef signed int s32;
typedef unsigned int u32;
typedef signed short s16;
typedef signed char s8;
typedef unsigned char u8;
typedef float f32;
typedef struct Selection8 {u8 reserved;u8 model;s8 type;s8 tune;s8 variant;u8 tail[3];} Selection8;
typedef struct Vehicle2056 {void *car;void *parameters;u8 opaque[1988];s8 mode;u8 tail[59];} Vehicle2056;
extern Selection8 D_80153E88[13];
extern Vehicle2056 D_8014A250[6];
extern u32 D_8002E580[];
extern u32 D_801174B4;
extern s16 D_8014A108;
extern s16 D_80142B08[13];
extern s8 D_80111604[],D_80111648[],D_8011168C[];
typedef struct Heap Heap;
extern u32 audio_output_setup(Heap *);
extern s32 func_80097694(s32,s8);
extern s32 audio_frame_sync(s32,s32,s32,s32,void *);
extern void func_800BB02C(s32,s32,void *);
extern void func_8039156C(void);
extern f32 func_8008B2E4(f32 max);
s32 func_8009768C(s32);
void audio_interrupt_handler(s32 select)
{
 s32 i;
 s32 j;
 s32 numLoadedCars;
 if (select) {
  func_8039156C();
  return;
 }
 numLoadedCars=0;
 for(i=0;i<13;i++) {
  if(i<6 && ((D_801174B4&8)||D_8014A250[i].mode==1)) {
again:
   D_80153E88[i].model=func_8008B2E4(13.0f);
   for(j=0;j<i;j++) {
    if(D_80153E88[i].model==D_80153E88[j].model)goto again;
   }
   D_80153E88[i].type=D_80111604[D_80153E88[i].model];
   D_80153E88[i].tune=D_80111648[D_80153E88[i].model];
   D_80153E88[i].variant=D_8011168C[D_80153E88[i].model];
   if(audio_output_setup(0)<D_8002E580[D_80153E88[i].model+88]+512) {
    for(j=0;j<13;j++) {
     if(func_8009768C(D_80153E88[i].model+88)<0 &&
        audio_output_setup(0)>=D_8002E580[D_80153E88[i].model+88]+512)goto load;
     D_80153E88[i].model=(D_80153E88[i].model+1)%13;
    }
    for(j=0;j<13;j++) {
     if(audio_output_setup(0)>=D_8002E580[D_80153E88[i].model+88]+512)goto load;
     D_80153E88[i].model=(D_80153E88[i].model+1)%13;
    }
    do { j=func_8008B2E4(numLoadedCars); }
    while(numLoadedCars>D_8014A108 && D_80153E88[j].model>=13);
    D_80153E88[i].model=D_80153E88[j].model;
    D_80142B08[i]=D_80142B08[j];
   } else {
load:
    D_80142B08[i]=audio_frame_sync(D_80153E88[i].model+88,1,0,0,0);
    numLoadedCars++;
   }
   func_800BB02C(i,D_80153E88[i].model,0);
  } else if(i<6 && D_8014A250[i].mode==2) {
   if(audio_output_setup(0)<D_8002E580[D_80153E88[i].model+88]+512) {
    for(D_80153E88[i].model=0;D_80153E88[i].model<13;D_80153E88[i].model++) {
     if(audio_output_setup(0)>=D_8002E580[D_80153E88[i].model+88]+512)goto load2;
    }
    D_80153E88[i].model=D_80153E88[0].model;
    D_80142B08[i]=D_80142B08[0];
    func_800BB02C(i,D_80153E88[i].model,0);
   } else {
load2:
    D_80142B08[i]=audio_frame_sync(D_80153E88[i].model+88,1,0,0,0);
    func_800BB02C(i,D_80153E88[i].model,0);
    numLoadedCars++;
   }
  } else {
   D_80153E88[i].model=D_80153E88[0].model;
   D_80142B08[i]=D_80142B08[0];
  }
 }
}
s32 func_8009768C(s32 waveNum) { return func_80097694(waveNum,-1); }
