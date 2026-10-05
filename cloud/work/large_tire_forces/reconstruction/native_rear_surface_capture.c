/* flags: -g0 -O3 -mips2 -G 0 -non_shared -Wab,-r4300_mul */
typedef unsigned char u8;
typedef signed char s8;
typedef signed short s16;
typedef unsigned int u32;
typedef struct Vec3 {float x,y,z;} Vec3;
typedef struct Basis {Vec3 rows[3];} Basis;
typedef struct Parameters {float values[40];} Parameters;
typedef struct VehicleModel {
 Parameters *parameters;u8 gap4[60];Vec3 velocity,angular;float wind_lateral,wind_vertical,drag;
 Vec3 tire_force[4],tire_velocity[4];u8 gap196[180];Basis tire_basis[4];
 u8 gap520[228];Basis transform,road_basis[4];u8 gap928[16];float steering;
 float wheel_torque[4];u8 gap964[12];float throttle;u8 gap980[36];s16 condition;
 u8 gap1018[54];u8 wheel_state[4][92];u8 gap1440[20];float slip;
 u8 gap1464[20];float suspension[4];u8 gap1500[48];int wheel_mode[4];
 u8 gap1564[44];void *direction;u8 gap1612[378];s16 index;
 u8 gap1992[4];s8 mode;u8 gap1997[7];u32 flags;u8 tail[48];
} VehicleModel;
typedef struct GameCar {u8 prefix[858];s8 disabled;u8 tail[93];} GameCar;
extern GameCar player_array[];
extern s8 D_80142726,D_80142DB0,D_8011128C[][13];
extern float D_801243CC,D_801243D0,D_801243D4;
extern float D_80114160,D_80114164,D_80114168,D_8011416C[];
extern void camera_collision_avoid(Vec3 *,Vec3 *,Vec3 *,float,Basis *,Basis *,Basis *,Vec3 *);
extern void camera_follow_target(VehicleModel *,Vec3 *,Basis *,void *,float,Vec3 *,float,float,float,float,float,float,int,float);
extern void func_800E1F80(VehicleModel *);
void func_800E23A4(VehicleModel *vehicle)
{
 /* Original forces1 declarations from drivsym.c; the N64 game-over
    drag block was removed, leaving its source-owned local declarations. */
 int poortract;
 int rear_surface;
 float airfact;
 float factor,speed,drag_base,drag_scale;
 airfact=1.0f;
 poortract=0;
 camera_collision_avoid(&vehicle->velocity,&vehicle->angular,(Vec3 *)&vehicle->parameters->values[28],vehicle->steering,&vehicle->transform,&vehicle->road_basis[0],&vehicle->tire_basis[0],&vehicle->tire_velocity[0]);
 camera_collision_avoid(&vehicle->velocity,&vehicle->angular,(Vec3 *)&vehicle->parameters->values[31],vehicle->steering,&vehicle->transform,&vehicle->road_basis[1],&vehicle->tire_basis[1],&vehicle->tire_velocity[1]);
 camera_collision_avoid(&vehicle->velocity,&vehicle->angular,(Vec3 *)&vehicle->parameters->values[34],0.0f,&vehicle->transform,&vehicle->road_basis[2],&vehicle->tire_basis[2],&vehicle->tire_velocity[2]);
 camera_collision_avoid(&vehicle->velocity,&vehicle->angular,(Vec3 *)&vehicle->parameters->values[37],0.0f,&vehicle->transform,&vehicle->road_basis[3],&vehicle->tire_basis[3],&vehicle->tire_velocity[3]);
 camera_follow_target(vehicle,&vehicle->tire_velocity[0],&vehicle->tire_basis[0],vehicle->wheel_state[0],vehicle->wheel_torque[0],&vehicle->tire_force[0],vehicle->suspension[0],vehicle->suspension[1],vehicle->parameters->values[6],vehicle->parameters->values[10],vehicle->parameters->values[12],vehicle->parameters->values[16],poortract,airfact);
 camera_follow_target(vehicle,&vehicle->tire_velocity[1],&vehicle->tire_basis[1],vehicle->wheel_state[1],vehicle->wheel_torque[1],&vehicle->tire_force[1],vehicle->suspension[1],vehicle->suspension[0],vehicle->parameters->values[7],vehicle->parameters->values[10],vehicle->parameters->values[13],vehicle->parameters->values[17],poortract,airfact);
 {
 float tsc;
 tsc=(float)1.0-vehicle->throttle;
 if(vehicle->tire_force[0].z<0.0f)vehicle->tire_force[0].z*=tsc;
 if(vehicle->tire_force[1].z<0.0f)vehicle->tire_force[1].z*=tsc;
 }
 rear_surface=vehicle->wheel_mode[2];
 poortract=((rear_surface==1 || rear_surface==2) && (vehicle->wheel_mode[3]==1 || vehicle->wheel_mode[0]==2));
 camera_follow_target(vehicle,&vehicle->tire_velocity[2],&vehicle->tire_basis[2],vehicle->wheel_state[2],vehicle->wheel_torque[2],&vehicle->tire_force[2],vehicle->suspension[2],vehicle->suspension[3],vehicle->parameters->values[8],vehicle->parameters->values[10],vehicle->parameters->values[14],vehicle->parameters->values[18],poortract,airfact);
 camera_follow_target(vehicle,&vehicle->tire_velocity[3],&vehicle->tire_basis[3],vehicle->wheel_state[3],vehicle->wheel_torque[3],&vehicle->tire_force[3],vehicle->suspension[3],vehicle->suspension[2],vehicle->parameters->values[9],vehicle->parameters->values[10],vehicle->parameters->values[15],vehicle->parameters->values[19],poortract,airfact);
 if(vehicle->direction)func_800E1F80(vehicle);
 if(!player_array[vehicle->index].disabled && vehicle->wheel_mode[0]==8 && vehicle->wheel_mode[1]==8 && vehicle->wheel_mode[2]==8 && vehicle->wheel_mode[3]==8) {
  vehicle->wind_lateral=((vehicle->transform.rows[0].x*D_80142726)*D_80142726)*80.0f;
  vehicle->wind_vertical=((vehicle->transform.rows[1].x*D_80142726)*D_80142726)*80.0f;
 } else {vehicle->wind_lateral=0.0f;vehicle->wind_vertical=0.0f;}
 if(!player_array[vehicle->index].disabled) speed=vehicle->velocity.z-(vehicle->transform.rows[2].x*D_80142726)*20.0f;
 else speed=vehicle->velocity.z;
 if(vehicle->flags&0x10) {drag_base=5000.0f;drag_scale=10.0f;}
 else {drag_base=30.0f;drag_scale=vehicle->parameters->values[22];}
 if(speed>0.0f)vehicle->drag=-(vehicle->parameters->values[23]+drag_base+(drag_scale*speed)*speed);
 else vehicle->drag=vehicle->parameters->values[23]+drag_base+(drag_scale*speed)*speed;
 if(vehicle->condition) {
  vehicle->drag*=D_8011416C[D_8011128C[vehicle->mode==2 ? vehicle->index+1 : 0][((u8 *)vehicle)[8]]];
 }
 if(vehicle->wheel_mode[3]==1 && vehicle->wheel_mode[2]==1) {
  if(D_80142DB0==2)factor=0.1f;else factor=1.0f-vehicle->slip;
  vehicle->drag+=(D_80114160*factor)*speed;
 } else if(vehicle->wheel_mode[3]==2 && vehicle->wheel_mode[2]==2) {
  if(D_80142DB0==2)factor=0.1f;else factor=1.0f-vehicle->slip;
  vehicle->drag+=(D_80114164*factor)*speed;
 } else {
  if(D_80142DB0==2)factor=0;else factor=vehicle->slip;
  vehicle->drag+=(D_80114168*factor)*speed;
 }
}
