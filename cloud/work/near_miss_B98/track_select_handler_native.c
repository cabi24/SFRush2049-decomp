/* flags: -g0 -O3 -mips2 -G 0 -non_shared -Wab,-r4300_mul */
typedef unsigned char u8;typedef signed char s8;typedef short s16;typedef unsigned int u32;
typedef struct Vec3 {float x,y,z;} Vec3;
typedef struct Vehicle2056 {
 u8 gap0[544];
 Vec3 velocity;
 Vec3 output;
 u8 gap568[180];
 float basis[9];
 u8 gap784[848];
 Vec3 position;
 u8 gap1644[48];
 float from_rotation[4];
 float to_rotation[4];
 u8 gap1724[8];
 s16 phase;
 u8 gap1734[2];
 float start;
 u8 gap1740[8];
 Vec3 from_position;
 u8 gap1760[52];
 float clock;
 u8 gap1816[174];
 s16 owner;
 u8 gap1992[2];
 s16 special;
 u8 gap1996[8];
 u32 flags;
 u8 gap2008[7];
 s8 inactive;
 u8 tail[40];
} Vehicle2056;
typedef struct Config8 {u8 prefix[7];u8 mode;} Config8;
extern Vec3 D_80161450[];
extern float D_80154390,D_80124128;
extern Config8 D_80153E88[];
extern s8 D_801427A1;
extern float func_8008B3C8(Vec3 *);
extern void func_800CFDEC(float *,float *,s16,float,float,float,float *);
extern void menu_vibration_test(float *,float *);
extern void math_utility(float *,float *);
extern void menu_video_settings(Vehicle2056 *);
extern float fabsf(float);
#pragma intrinsic (fabsf)
void track_select_handler(Vehicle2056 *vehicle)
{
 Vec3 delta,result;
 float quaternion[4],matrix[9];
 float height,elapsed,step,scale;
 Vec3 *middle;
 s16 owner=vehicle->owner,i;
 if(vehicle->phase==0) {
  delta.x=vehicle->from_position.x-vehicle->position.x;
  delta.y=vehicle->from_position.y-vehicle->position.y;
  delta.z=vehicle->from_position.z-vehicle->position.z;
  height=func_8008B3C8(&delta)/8.0f;
  if(height>100.0f)height=100.0f;
  middle=&D_80161450[owner];
  func_800CFDEC((float *)&vehicle->from_position,(float *)&vehicle->position,3,0.0f,D_80154390,D_80154390/2.0f,(float *)middle);
  middle->y+=height;vehicle->flags&=~8;
 }
 vehicle->phase++;
 elapsed=vehicle->clock-vehicle->start;
 if(elapsed>D_80154390) {
  if(vehicle->flags&0x10)vehicle->flags&=~0x10;
  vehicle->phase=-2;
  if(D_80153E88[owner].mode==6 || vehicle->special)vehicle->inactive=0;
  return;
 }
 middle=&D_80161450[owner];
 if(vehicle->flags&0x10)vehicle->flags&=~0x10;
 if(elapsed<D_80154390/2.0f) {
  step=((2.0f*elapsed)*elapsed)/D_80154390;
  func_800CFDEC((float *)&vehicle->from_position,(float *)middle,3,0.0f,D_80154390/2.0f,step,(float *)&result);
  func_800CFDEC(vehicle->from_rotation,vehicle->to_rotation,4,0.0f,D_80154390/2.0f,step,quaternion);
  menu_vibration_test(quaternion,matrix);
  scale=1.0f-(2.0f*step)/D_80154390;
  if(D_80153E88[owner].mode==6) {
   for(i=0;i<3;i++) {
    ((float *)&vehicle->velocity)[i]*=scale;
    if(fabsf(((float *)&vehicle->velocity)[i])<D_80124128)((float *)&vehicle->velocity)[i]=0.0f;
   }
  }
  math_utility(matrix,vehicle->basis);
  if(D_801427A1)menu_video_settings(vehicle);
 } else {
  func_800CFDEC((float *)middle,(float *)&vehicle->position,3,D_80154390/2.0f,D_80154390,elapsed,(float *)&result);
 }
 vehicle->output.x=result.x;vehicle->output.y=result.y;vehicle->output.z=result.z;
}
