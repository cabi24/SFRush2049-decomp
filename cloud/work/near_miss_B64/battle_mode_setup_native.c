/* flags: -g0 -O3 -mips2 -G 0 -non_shared -Wab,-r4300_mul */
typedef float f32;typedef unsigned char u8;typedef signed char s8;typedef signed short s16;
typedef struct Vec3 {f32 x,y,z;} Vec3;
typedef struct Basis {f32 values[9];} Basis;
typedef struct Record2056 {u8 prefix[76];Vec3 velocity;u8 gap[456];Vec3 axis,position;u8 gap2[180];Basis basis;u8 gap3[1028];f32 timestamp;u8 gap4[124];Vec3 result;Basis result_basis;u8 gap5[4];s16 active;u8 gap6[42];f32 scale;u8 tail[16];} Record2056;
typedef struct Model952 {u8 prefix[857];s8 state;u8 tail[94];} Model952;
extern Record2056 D_8014A250[];
extern Model952 player_array[];
extern void math_utility(Basis *,Basis *),sound_position_set(Vec3 *,Basis *);
void battle_mode_setup(f32 now) {
 int i;
 Record2056 *record;
 f32 scale;
 Vec3 delta;
 for(i=0;i<6;i++) {
  record=&D_8014A250[i];
  if(record->active && player_array[i].state<2) {
   scale=(now-record->timestamp)*record->scale;
   delta.x=record->axis.x*scale;
   delta.y=record->axis.y*scale;
   delta.z=record->axis.z*scale;
   record->result.x=delta.x+record->position.x;
   record->result.y=delta.y+record->position.y;
   record->result.z=delta.z+record->position.z;
   math_utility(&record->basis,&record->result_basis);
   delta.x=record->velocity.x*scale;
   delta.y=record->velocity.y*scale;
   delta.z=record->velocity.z*scale;
   sound_position_set(&delta,&record->result_basis);
  }
 }
}
