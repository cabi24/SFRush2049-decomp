/* flags: -g0 -O3 -mips2 -G 0 -non_shared -Wab,-r4300_mul */
typedef unsigned char u8;typedef signed char s8;typedef signed short s16;typedef unsigned int u32;typedef float f32;
typedef struct Resource {u8 *data;} Resource;
typedef struct Object {u8 opaque[44];Resource *resource;} Object;
typedef struct Handle {Object *object;} Handle;
typedef struct Player {u8 index,selector;u8 opaque[70];Handle *handle;} Player;
typedef struct Model952 {u8 opaque[264];f32 distance;u8 rest[684];} Model952;
extern s8 D_8014978C,D_80152570;
extern s16 active_player_count;
extern Player input_rec0[];
extern Object *D_80146150[];
extern u8 D_80150F88[];
extern Model952 player_array[];
extern void func_800CD8EC(Handle *,u8);
void place_cars_in_order(void) {
 int player,which;
 int table=D_8014978C,refresh=D_8014978C;
 Player *record;
 u8 *stats;
 if(D_80152570) {table+=6;refresh+=19;}
 for(player=0;player<active_player_count;player++) {
  record=&input_rec0[player];
  if(record->handle==0)record->handle=(Handle *)&D_80146150[record->selector];
  if(record->handle->object->resource==0)return;
  for(which=0;which<2;which++) {
   if(which==0)stats=record->handle->object->resource->data+table*96+140;
   else stats=D_80150F88+table*96;
   *(u32 *)(stats+88)=(u32)((f32)*(u32 *)(stats+88)+player_array[record->index].distance/528.0f);
  }
  func_800CD8EC(record->handle,(u8)refresh);
 }
}
