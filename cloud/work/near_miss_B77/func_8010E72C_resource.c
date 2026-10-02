/* flags: -g0 -O3 -mips2 -G 0 -non_shared -Wab,-r4300_mul */
typedef unsigned char u8;typedef signed char s8;typedef signed short s16;typedef unsigned int u32;typedef float f32;
typedef struct Basis {f32 m[9];} Basis;
typedef struct Actor96 {u8 prefix[4],flags,opaque[11];s16 index;u8 gap[2];Basis basis;f32 position[3];u8 gap2[22];s16 state;s8 player;u8 tail[3];} Actor96;
typedef struct Model952 {u8 prefix[20];f32 direction[3];u8 tail[920];} Model952;
typedef struct Node24 {struct Node24 *next;s16 state;u8 gap[6];Actor96 *owner;f32 time;u32 resource;} Node24;
typedef struct Row48 {u32 resource;u8 opaque[12];u32 event;u8 tail[28];} Row48;
extern Row48 D_8011753C[];
extern f32 D_801249D0;
extern Node24 *D_801391F0;
extern Model952 player_array[];
extern Node24 *func_80090284(Actor96 *);
extern void vector_normalize_length(f32 *,Basis *),math_utility(Basis *,Basis *);
extern int stat_lap_split(u32,int,f32 *,u8);
void func_8010E72C(Actor96 *actor) {
 Basis basis;
 Node24 *node;
 u32 resource;
 node=func_80090284(actor);
 if(node!=0) {
  node->state=0;
  resource=D_8011753C[actor->index].resource;
  node->owner=actor;
  node->resource=resource;
  node->time=D_801249D0;
  actor->state=4;
  actor->flags&=~6;
  vector_normalize_length(player_array[actor->player].direction,&basis);
  math_utility(&basis,&actor->basis);
  node->next=D_801391F0;
  D_801391F0=node;
  stat_lap_split(D_8011753C[actor->index].event,actor->player,actor->position,2);
 }
}
