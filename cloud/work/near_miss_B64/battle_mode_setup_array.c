/* flags: -g0 -O3 -mips2 -G 0 -non_shared -Wab,-r4300_mul */
typedef float f32;typedef unsigned char u8;typedef signed char s8;typedef signed short s16;
typedef f32 Vec3[3];
typedef struct Basis {f32 values[9];} Basis;
typedef struct Record2056 {u8 prefix[76];Vec3 velocity;u8 gap[456];Vec3 axis,position;u8 gap2[180];Basis basis;u8 gap3[1028];f32 timestamp;u8 gap4[124];Vec3 result;Basis result_basis;u8 gap5[4];s16 active;u8 gap6[42];f32 scale;u8 tail[16];} Record2056;
typedef struct Model952 {u8 prefix[857];s8 state;u8 tail[94];} Model952;
extern Record2056 D_8014A250[];
extern Model952 player_array[];
extern void math_utility(Basis *,Basis *),sound_position_set(f32 *,Basis *);
void battle_mode_setup(f32 now) {
 int i;
 Record2056 *record;
 f32 scale;
 Vec3 delta;
 for(i=0;i<6;i++) {
  record=&D_8014A250[i];
  if(record->active && player_array[i].state<2) {
   scale=(now-record->timestamp)*record->scale;
   delta[0]=record->axis[0]*scale;
   delta[1]=record->axis[1]*scale;
   delta[2]=record->axis[2]*scale;
   record->result[0]=delta[0]+record->position[0];
   record->result[1]=delta[1]+record->position[1];
   record->result[2]=delta[2]+record->position[2];
   math_utility(&record->basis,&record->result_basis);
   delta[0]=record->velocity[0]*scale;
   delta[1]=record->velocity[1]*scale;
   delta[2]=record->velocity[2]*scale;
   sound_position_set(delta,&record->result_basis);
  }
 }
}
