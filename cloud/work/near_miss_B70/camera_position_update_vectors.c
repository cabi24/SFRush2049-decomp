/* flags: -g0 -O3 -mips2 -G 0 -non_shared -Wab,-r4300_mul */
typedef float f32;typedef unsigned char u8;typedef signed char s8;typedef signed short s16;
typedef struct Model952 {u8 prefix[8];f32 position[3];u8 opaque[932];} Model952;
typedef struct Vec3 {f32 x,y,z;} Vec3;
typedef struct Basis {f32 m[9];} Basis;
extern u8 D_801613C0[4][4];
extern s8 D_801613A8;
extern int gameplay_mode;
extern s16 active_player_count;
extern Model952 player_array[];
extern void *input_deadzone_apply(f32 *,f32 *,Basis *,f32,int,int);
void camera_position_update(void) {
 Vec3 first,second;
 Basis basis;
 int i,j;
 for(i=0;i<16;i++)((u8 *)D_801613C0)[i]=0;
 if(D_801613A8 && gameplay_mode==6) {
  for(i=0;i<active_player_count-1;i++) {
   first.x=player_array[i].position[0];
   first.y=player_array[i].position[1]+5.0f;
   first.z=player_array[i].position[2];
   for(j=1;j<active_player_count;j++) {
    second.x=player_array[j].position[0];
    second.y=player_array[j].position[1]+5.0f;
    second.z=player_array[j].position[2];
    if(input_deadzone_apply(&first.x,&second.x,&basis,0.5f,0,0xFFFF)) {
     D_801613C0[i][j]=1;
     D_801613C0[j][i]=1;
    }
   }
  }
 }
}
