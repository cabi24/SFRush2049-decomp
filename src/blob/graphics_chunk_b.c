/* flags: -g0 -O3 -mips2 -G 0 -non_shared -Wab,-r4300_mul */
typedef unsigned char u8;typedef signed char s8;typedef unsigned short u16;typedef signed short s16;typedef float f32;
typedef struct Resource {u8 *data;} Resource;
typedef struct Object {u8 opaque[44];Resource *resource;} Object;
typedef struct Handle {Object *object;} Handle;
typedef struct Player {u8 index,selector;u8 opaque[62];u16 count;u8 rest[6];Handle *handle;} Player;
typedef struct Model952 {u8 opaque[240];f32 time;u8 rest[708];} Model952;
typedef struct Stats24 {int unknown;f32 minimum,total;u16 count,finished,sum,enabled,enabled_finished;u8 tail[2];} Stats24;
extern s8 D_8014978C,D_80142760;
extern s16 active_player_count;
extern Player input_rec0[];
extern Object *D_80146150[];
extern u8 D_80144018[];
extern Stats24 D_801515F8[];
extern Model952 player_array[];
extern void func_800CD8EC(Handle *,u8);
void graphics_chunk_b(void) {
 int player,which,table=D_8014978C-18;
 Player *record;
 Stats24 *stats;
 Model952 *model;
 for(player=0;player<active_player_count;player++) {
  record=&input_rec0[player];
  if(record->handle==0)record->handle=(Handle *)&D_80146150[record->selector];
  if(record->handle->object->resource==0)return;
  for(which=0;which<2;which++) {
   if(which==0)stats=(Stats24 *)(record->handle->object->resource->data+table*24+1644);
   else stats=&D_801515F8[table];
   model=&player_array[record->index];
   stats->total+=model->time;
   stats->count++;
   if(D_80144018[player]==1) {
    stats->finished++;
    if(stats->minimum==0.0f || model->time<stats->minimum)stats->minimum=model->time;
   }
   stats->sum+=record->count;
   if(D_80142760) {
    stats->enabled++;
    if(D_80144018[player]==1)stats->enabled_finished++;
   }
  }
  func_800CD8EC(record->handle,(u8)D_8014978C);
 }
}
