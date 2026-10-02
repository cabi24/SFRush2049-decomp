/* flags: -g0 -O2 -mips2 -G 0 -non_shared -Wab,-r4300_mul */
typedef unsigned char u8;
typedef signed char s8;
typedef signed short s16;
typedef unsigned int u32;
typedef struct Vec3 {float x,y,z;} Vec3;
typedef struct Basis {Vec3 rows[3];} Basis;
typedef struct Parameters {float values[40];} Parameters;
typedef struct Vehicle2056 {
 Parameters *parameters;u8 gap4[60];Vec3 velocity,angular;float wind_x,wind_y,drag;
 Vec3 wheel_impulse[4],wheel_position[4];u8 gap196[180];Basis wheel_transform[4];
 u8 gap520[228];Basis transform,wheel_basis[4];u8 gap928[16];float steering;
 float wheel_stroke[4];u8 gap964[12];float damping;u8 gap980[36];s16 condition;
 u8 gap1018[54];u8 wheel_state[4][92];u8 gap1440[20];float slip;
 u8 gap1464[20];float friction[4];u8 gap1500[48];int wheel_mode[4];
 u8 gap1564[44];void *direction;u8 gap1612[378];s16 index;
 u8 gap1992[4];s8 mode;u8 gap1997[7];u32 flags;u8 tail[48];
} Vehicle2056;
typedef struct Model952 {u8 prefix[858];s8 disabled;u8 tail[93];} Model952;
extern Model952 player_array[];
extern s8 D_80142726,D_80142DB0,D_8011128C[][13];
extern float D_801243CC,D_801243D0,D_801243D4;
extern float D_80114160,D_80114164,D_80114168,D_8011416C[];
extern void camera_collision_avoid(Vec3 *,Vec3 *,Vec3 *,float,Basis *,Basis *,Basis *,Vec3 *);
extern void camera_follow_target(Vehicle2056 *,Vec3 *,Basis *,void *,float,Vec3 *,float,float,float,float,float,float,int,float);
extern void func_800E1F80(Vehicle2056 *);
void func_800E23A4(Vehicle2056 *vehicle)
{
 Vec3 *velocity=&vehicle->velocity,*angular=&vehicle->angular;
 Basis *transform=&vehicle->transform;
 Vec3 *point0=&vehicle->wheel_position[0],*point1=&vehicle->wheel_position[1];
 Vec3 *point2=&vehicle->wheel_position[2],*point3=&vehicle->wheel_position[3];
 Basis *matrix0=&vehicle->wheel_transform[0],*matrix1=&vehicle->wheel_transform[1];
 Basis *matrix2=&vehicle->wheel_transform[2],*matrix3=&vehicle->wheel_transform[3];
 float factor,speed,drag_base,drag_scale;
 int linked,row;
 camera_collision_avoid(velocity,angular,(Vec3 *)&vehicle->parameters->values[28],vehicle->steering,transform,&vehicle->wheel_basis[0],matrix0,point0);
 camera_collision_avoid(velocity,angular,(Vec3 *)&vehicle->parameters->values[31],vehicle->steering,transform,&vehicle->wheel_basis[1],matrix1,point1);
 camera_collision_avoid(velocity,angular,(Vec3 *)&vehicle->parameters->values[34],0.0f,transform,&vehicle->wheel_basis[2],matrix2,point2);
 camera_collision_avoid(velocity,angular,(Vec3 *)&vehicle->parameters->values[37],0.0f,transform,&vehicle->wheel_basis[3],matrix3,point3);
 camera_follow_target(vehicle,point0,matrix0,vehicle->wheel_state[0],vehicle->wheel_stroke[0],&vehicle->wheel_impulse[0],vehicle->friction[0],vehicle->friction[1],vehicle->parameters->values[6],vehicle->parameters->values[10],vehicle->parameters->values[12],vehicle->parameters->values[16],0,1.0f);
 camera_follow_target(vehicle,point1,matrix1,vehicle->wheel_state[1],vehicle->wheel_stroke[1],&vehicle->wheel_impulse[1],vehicle->friction[1],vehicle->friction[0],vehicle->parameters->values[7],vehicle->parameters->values[10],vehicle->parameters->values[13],vehicle->parameters->values[17],0,1.0f);
 factor=1.0f-vehicle->damping;
 if(vehicle->wheel_impulse[0].z<0.0f)vehicle->wheel_impulse[0].z*=factor;
 if(vehicle->wheel_impulse[1].z<0.0f)vehicle->wheel_impulse[1].z*=factor;
 linked=((vehicle->wheel_mode[2]==1 || vehicle->wheel_mode[2]==2) && (vehicle->wheel_mode[3]==1 || vehicle->wheel_mode[0]==2));
 camera_follow_target(vehicle,point2,matrix2,vehicle->wheel_state[2],vehicle->wheel_stroke[2],&vehicle->wheel_impulse[2],vehicle->friction[2],vehicle->friction[3],vehicle->parameters->values[8],vehicle->parameters->values[10],vehicle->parameters->values[14],vehicle->parameters->values[18],linked,1.0f);
 camera_follow_target(vehicle,point3,matrix3,vehicle->wheel_state[3],vehicle->wheel_stroke[3],&vehicle->wheel_impulse[3],vehicle->friction[3],vehicle->friction[2],vehicle->parameters->values[9],vehicle->parameters->values[10],vehicle->parameters->values[15],vehicle->parameters->values[19],linked,1.0f);
 if(vehicle->direction)func_800E1F80(vehicle);
 if(!player_array[vehicle->index].disabled && vehicle->wheel_mode[0]==8 && vehicle->wheel_mode[1]==8 && vehicle->wheel_mode[2]==8 && vehicle->wheel_mode[3]==8) {
  vehicle->wind_x=((vehicle->transform.rows[0].x*D_80142726)*D_80142726)*80.0f;
  vehicle->wind_y=((vehicle->transform.rows[1].x*D_80142726)*D_80142726)*80.0f;
 } else {vehicle->wind_x=0.0f;vehicle->wind_y=0.0f;}
 if(!player_array[vehicle->index].disabled) speed=vehicle->velocity.z-(vehicle->transform.rows[2].x*D_80142726)*20.0f;
 else speed=vehicle->velocity.z;
 if(vehicle->flags&0x10) {drag_base=D_801243CC;drag_scale=10.0f;}
 else {drag_base=30.0f;drag_scale=vehicle->parameters->values[22];}
 if(speed>0.0f)vehicle->drag=-(vehicle->parameters->values[23]+drag_base+(drag_scale*speed)*speed);
 else vehicle->drag=vehicle->parameters->values[23]+drag_base+(drag_scale*speed)*speed;
 if(vehicle->condition) {
  row=0;
  if(vehicle->mode==2)row=vehicle->index+1;
  vehicle->drag*=D_8011416C[D_8011128C[row][((u8 *)vehicle)[8]]];
 }
 if(vehicle->wheel_mode[3]==1 && vehicle->wheel_mode[2]==1) {
  if(D_80142DB0==2)factor=D_801243D0;else factor=1.0f-vehicle->slip;
  vehicle->drag+=(D_80114160*factor)*speed;
 } else if(vehicle->wheel_mode[3]==2 && vehicle->wheel_mode[2]==2) {
  if(D_80142DB0==2)factor=D_801243D4;else factor=1.0f-vehicle->slip;
  vehicle->drag+=(D_80114164*factor)*speed;
 } else {
  if(D_80142DB0==2)factor=0.0f;else factor=vehicle->slip;
  vehicle->drag+=(D_80114168*factor)*speed;
 }
}
