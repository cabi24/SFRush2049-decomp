/* flags: -g0 -O3 -mips2 -G 0 -non_shared -Wab,-r4300_mul */
typedef unsigned char u8;typedef unsigned int u32;typedef signed short s16;
typedef struct Handle Handle;
typedef struct Object {Handle *next;u32 secondary_id,primary_id;} Object;
struct Handle {Object *object;};
typedef struct List16 {u8 indirect,doubly;u8 gap[2];int count;Handle *head,*tail;} List16;
typedef struct Player76 {u8 opaque[72];Handle *handle;} Player76;
extern List16 D_8012E6D8,D_80152020;
extern Player76 input_rec0[];
extern s16 active_player_count;
extern void audio_channel_priority(Handle *),func_8009211C(List16 *,Handle *);
void audio_volume_pan(u32 identifier) {
 Handle *node;
 Player76 *player;
 for(node=D_8012E6D8.head;node!=0;node=node->object->next) {
  if(node->object->primary_id==identifier) {
   for(player=input_rec0;player<input_rec0+active_player_count;player++) {
    if(player->handle==node)player->handle=0;
   }
   audio_channel_priority(node);
   func_8009211C(&D_8012E6D8,node);
   return;
  }
 }
 for(node=D_80152020.head;node!=0;node=node->object->next) {
  if(node->object->secondary_id==identifier) {
   func_8009211C(&D_80152020,node);
   node->object=0;
   return;
  }
 }
}
