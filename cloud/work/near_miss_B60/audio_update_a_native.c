/* flags: -g0 -O3 -mips2 -G 0 -non_shared -Wab,-r4300_mul */
typedef unsigned char u8;typedef signed char s8;typedef unsigned short u16;typedef signed short s16;typedef unsigned int u32;
typedef struct Resource {u8 *data;} Resource;
typedef struct Object {u8 opaque[44];Resource *resource;} Object;
typedef struct Handle {Object *object;} Handle;
typedef struct Player {u8 index,selector;u8 opaque[62];u16 count;u8 rest[6];Handle *handle;} Player;
typedef struct Stats64 {u32 unknown,peak,average,total;u16 ticks,count;u32 metrics[10];u8 tail[4];} Stats64;
typedef struct Metrics120 {u8 opaque[8];u32 peak;u8 gap[8];int total;u8 gap2[48];s16 metrics[10];u8 tail[28];} Metrics120;
extern s8 D_8014978C;
extern s16 active_player_count;
extern Player input_rec0[];
extern Object *D_80146150[];
extern int D_80140804;
extern Stats64 D_80151410[];
extern Metrics120 D_80152038[];
extern void func_800CD8EC(Handle *,u8);
void audio_update_a(void) {
 int player,which,i,table=D_8014978C-14;
 Player *record;
 Stats64 *stats;
 Metrics120 *metrics;
 u32 average;
 for(player=0;player<active_player_count;player++) {
  record=&input_rec0[player];
  if(record->handle==0)record->handle=(Handle *)&D_80146150[record->selector];
  if(record->handle->object->resource==0)return;
  metrics=&D_80152038[player];
  for(which=0;which<2;which++) {
   if(which==0)stats=(Stats64 *)(record->handle->object->resource->data+table*64+1292);
   else stats=&D_80151410[table];
   if(stats->peak<metrics->peak)stats->peak=metrics->peak;
   average=metrics->total/D_80140804;
   if(stats->average<average)stats->average=average;
   stats->total+=metrics->total;
   stats->ticks+=D_80140804;
   stats->count+=record->count;
   for(i=0;i<10;i++)stats->metrics[i]+=metrics->metrics[i];
  }
  func_800CD8EC(record->handle,(u8)D_8014978C);
 }
}
