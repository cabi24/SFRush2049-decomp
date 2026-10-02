/* flags: -g0 -O3 -mips2 -G 0 -non_shared -Wab,-r4300_mul */
typedef unsigned char u8;typedef signed char s8;typedef unsigned short u16;typedef signed short s16;typedef unsigned int u32;typedef float f32;
typedef struct Resource {u8 *data;} Resource;
typedef struct Object {u8 prefix[8];void *context;u8 opaque[32];Resource *resource;} Object;
typedef struct Handle {Object *object;} Handle;
typedef struct Player {u8 index,selector;u8 opaque[62];u16 count;u8 rest[6];Handle *handle;} Player;
typedef struct Model952 {u8 opaque[239];s8 eligible;f32 time;u8 gap[20];f32 distance;u8 tail[684];} Model952;
typedef struct Stats96 {u8 opaque[44];f32 ranks[5],sum;u16 flagged,count;u8 gap[6];u16 samples;u8 gap2[4];u16 updates;u8 gap3[2];u32 distance;u8 tail[4];} Stats96;
typedef struct Ranks60 {u8 opaque[40];Handle *ranks[5];} Ranks60;
extern s8 D_8014978C,D_80152570;
extern s16 active_player_count;
extern Player input_rec0[];
extern Object *D_80146150[];
extern u8 D_80144018[];
extern Stats96 D_80150F88[];
extern Model952 player_array[];
extern Ranks60 D_80151690[];
extern s8 D_80151AC0[];
extern f32 D_80149A78[][8];
extern void func_800CD8EC(Handle *,u8);
void assign_drones(void) {
 int player,which,i,j,sample;
 int table=D_8014978C,refresh=D_8014978C;
 Player *record;
 Stats96 *stats;
 Model952 *model;
 f32 time;
 if(D_80152570) {table+=6;refresh+=19;}
 for(player=0;player<active_player_count;player++) {
  record=&input_rec0[player];
  if(record->handle==0)record->handle=(Handle *)&D_80146150[record->selector];
  if(record->handle->object->resource==0)return;
  for(which=0;which<2;which++) {
   if(which==0)stats=(Stats96 *)(record->handle->object->resource->data+table*96+140);
   else stats=&D_80150F88[table];
   model=&player_array[record->index];
   if(model->eligible) {
    time=model->time;
    for(i=0;i<5;i++) {
     if(stats->ranks[i]==0.0f || time<stats->ranks[i]) {
      for(j=4;j>i;j--) {
       stats->ranks[j]=stats->ranks[j-1];
       if(which==1) {
        D_80151AC0[10+j]=D_80151AC0[9+j];
        D_80151690[table].ranks[j]=D_80151690[table].ranks[j-1];
       }
      }
      stats->ranks[i]=time;
      if(which==1) {
       D_80151AC0[10+j]=player;
       if(record->handle->object->context!=0)D_80151690[table].ranks[i]=record->handle;
      }
      break;
     }
    }
   }
   stats->count++;
   stats->updates++;
   stats->samples+=record->count;
   stats->flagged+=D_80144018[player];
   for(sample=0;sample<D_80144018[player];sample++)stats->sum+=D_80149A78[player][sample];
   stats->distance=(u32)((f32)stats->distance+model->distance/528.0f);
  }
  func_800CD8EC(record->handle,(u8)refresh);
 }
}
