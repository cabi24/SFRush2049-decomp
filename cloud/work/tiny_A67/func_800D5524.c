/* flags: -g0 -O2 -mips2 -G 0 -non_shared */
typedef unsigned char u8;typedef signed char s8;typedef signed short s16;typedef unsigned int u32;typedef int s32;typedef float f32;
typedef struct Record20 {s32 handle;f32 state[4];} Record20;
typedef struct Group84 {u32 pad0;Record20 pair[2];u8 pad44[20];Record20 last;} Group84;
typedef struct Group60 {Record20 records[3];} Group60;
typedef struct Car {u8 pad0[1990];s16 index;u8 pad1992[4];s8 mode;} Car;
extern s8 D_8010FFC4[],D_8010FFCC[];extern Group84 D_80140420[];
extern Record20 D_80140640[];extern Group60 D_801406C0[];
extern s32 D_80140AE0[],D_801407E0[],D_801407C0[];extern s16 D_80140A08[];extern f32 D_80140B10[];
extern void results_screen_update(u32);extern void scheduler_recv(u32);extern void player_conditional_call(Record20 *);
void func_800D5524(Car *car) {
 s16 index=car->index;Record20 *record;s32 offset;
 if(!D_8010FFC4[index])return;
 D_8010FFC4[index]=0;D_8010FFCC[index]=(index&1)==0;
 for(offset=0;offset<2;offset++) {
  record=&D_80140420[index].pair[offset];
  if(record->handle!=-1) {
   if(car->mode!=2)results_screen_update(record->handle);else scheduler_recv(record->handle);
   player_conditional_call(record);
  }
 }
 record=&D_80140420[index].last;
 if(record->handle!=-1) {
  if(car->mode!=2)results_screen_update(record->handle);else scheduler_recv(record->handle);
  player_conditional_call(record);
 }
 record=&D_80140640[index];
 if(car->mode!=2)results_screen_update(record->handle);else scheduler_recv(record->handle);
 player_conditional_call(record);
 if(car->mode==2) {
  record=&D_801406C0[index].records[0];scheduler_recv(record->handle);player_conditional_call(record);
  record=&D_801406C0[index].records[1];scheduler_recv(record->handle);player_conditional_call(record);
  record=&D_801406C0[index].records[2];scheduler_recv(record->handle);player_conditional_call(record);
  scheduler_recv(D_80140AE0[index]);D_80140AE0[index]=-1;
  D_80140A08[index]=0;D_80140B10[index]=0.0f;
  scheduler_recv(D_801407E0[index]);D_801407E0[index]=-1;
  scheduler_recv(D_801407C0[index]);D_801407C0[index]=-1;
 }
}
