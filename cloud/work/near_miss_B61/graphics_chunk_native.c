/* flags: -g0 -O3 -mips2 -G 0 -non_shared -Wab,-r4300_mul */
typedef unsigned char u8;typedef signed char s8;typedef unsigned short u16;typedef signed short s16;
typedef struct Resource {u8 *data;} Resource;
typedef struct Object {u8 opaque[44];Resource *resource;} Object;
typedef struct Handle {Object *object;} Handle;
typedef struct Player {u8 index,selector;u8 opaque[70];Handle *handle;} Player;
typedef struct Stats12 {int unknown;u16 count,wins,others,absolute;} Stats12;
extern s8 D_8014978C,D_80143F54;
extern s16 active_player_count;
extern Player input_rec0[];
extern Object *D_80146150[];
extern s8 D_80149428[][4],D_8012E67C[],D_80150B68[];
extern Stats12 D_80151578[];
extern void func_800CD8EC(Handle *,u8);
void graphics_chunk(void) {
 int player,which,other,value,table=D_8014978C-6;
 Player *record;
 Stats12 *stats;
 for(player=0;player<active_player_count;player++) {
  record=&input_rec0[player];
  if(record->handle==0)record->handle=(Handle *)&D_80146150[record->selector];
  if(record->handle->object->resource==0)return;
  for(which=0;which<2;which++) {
   if(which==0)stats=(Stats12 *)(record->handle->object->resource->data+table*12+1548);
   else stats=&D_80151578[table];
   for(other=0;other<active_player_count;other++) {
    if(other!=player)stats->others+=D_80149428[player][other];
    value=D_80149428[other][player];
    stats->absolute+=value<0?-value:value;
   }
   stats->count++;
   if(player==D_80143F54 || D_8012E67C[player]==D_8012E67C[D_80143F54] || D_80150B68[player]==1)stats->wins++;
  }
  func_800CD8EC(record->handle,(u8)D_8014978C);
 }
}
