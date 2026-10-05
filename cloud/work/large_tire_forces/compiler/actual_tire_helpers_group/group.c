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
extern void camera_collision_avoid(const Vec3 *,const Vec3 *,const Vec3 *,float,const Basis *,const Basis *,Basis *,Vec3 *);
extern void camera_follow_target(VehicleModel *,Vec3 *,Basis *,void *,float,Vec3 *,float,float,float,float,float,float,int,float);
extern void func_800E1F80(VehicleModel *);
void func_800E23A4(VehicleModel *vehicle)
{
 /* Original forces1 declarations from drivsym.c; the N64 game-over
    drag block was removed, leaving its source-owned local declarations. */
 int poortract;
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
 poortract=((vehicle->wheel_mode[2]==1 || vehicle->wheel_mode[2]==2) && (vehicle->wheel_mode[3]==1 || vehicle->wheel_mode[0]==2));
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

typedef float f32;typedef int s32;
extern void func_800A61B0(const Vec3 *,Vec3 *,const Basis *);
extern f32 cosf(f32),sinf(f32);
void camera_collision_avoid(const Vec3 *origin,const Vec3 *first,const Vec3 *second,f32 angle,const Basis *transform,const Basis *source,Basis *output,Vec3 *position) {
 Vec3 point;
 point.x=second->y*first->z-first->y*second->z;
 point.y=second->z*first->x-first->z*second->x;
 point.z=second->x*first->y-first->x*second->y;
 point.x=origin->x+point.x;
 point.y=origin->y+point.y;
 point.z=origin->z+point.z;
 func_800A61B0(&source->rows[1],&output->rows[1],transform);
 output->rows[0].x=cosf(angle);
 output->rows[0].y=0.0f;
 output->rows[0].z=-sinf(angle);
 output->rows[2].x=output->rows[0].y*output->rows[1].z-output->rows[1].y*output->rows[0].z;
 output->rows[2].y=output->rows[0].z*output->rows[1].x-output->rows[1].z*output->rows[0].x;
 output->rows[2].z=output->rows[0].x*output->rows[1].y-output->rows[1].x*output->rows[0].y;
 output->rows[0].x=output->rows[1].y*output->rows[2].z-output->rows[2].y*output->rows[1].z;
 output->rows[0].y=output->rows[1].z*output->rows[2].x-output->rows[2].z*output->rows[1].x;
 output->rows[0].z=output->rows[1].x*output->rows[2].y-output->rows[2].x*output->rows[1].y;
 func_800A61B0(&point,position,output);
}

#define FIELD(e,t,o) (*(t *)((u8 *)(e)+(o)))
extern f32 D_80111130[],D_80110F80[];
extern f32 D_80123E10,D_80123E14,D_80123E18,D_80123E1C,D_80123E20,D_80123E24;
extern s8 D_80142DB0;
f32 fabsf(f32);
#pragma intrinsic(fabsf)
void camera_dolly(void *,f32 *,f32,f32,void *,f32 *,f32 *);
void func_8009E820(f32 *,f32 *,void *);
void camera_follow_target(VehicleModel *model,Vec3 *tirev,Basis *tireuv,void *tire,f32 torque,Vec3 *forcevec,f32 suscomp,f32 otsuscomp,f32 springrate,f32 arspringrate,f32 cdamping,f32 rdamping,s32 poortract,f32 airfact) {
    f32 arforce,damping,normal,sideforce,traction;
    f32 tireforcevec[3];
    f32 rate;
    if(suscomp>0.0f && otsuscomp>0.0f)arforce=(suscomp-otsuscomp)*arspringrate;
    else arforce=0.0f;
    if(tirev->y<0.0f)damping=cdamping;else damping=rdamping;
    if(suscomp>10.0f){
        if(tirev->y<1.0f)tireforcevec[1]=(1.0f-tirev->y)*FIELD(model,f32,1472)*-0.25f*FIELD(model,f32,1592);
        else tireforcevec[1]=arforce+suscomp*springrate-damping*tirev->y;
    }else if(suscomp>0.0f)tireforcevec[1]=arforce+suscomp*springrate-damping*tirev->y;
    else tireforcevec[1]=0.0f;
    rate=FIELD(model,f32,1592);
    normal=tireforcevec[1]*(rate*(rate*D_80123E10-D_80123E14));
    tireforcevec[1]=normal;
    if(normal<0.0f){normal=0.0f;tireforcevec[1]=0.0f;}
    else if(normal>FIELD(model,f32,1468))normal=FIELD(model,f32,1468);
    camera_dolly(model,(f32 *)tirev,normal,torque,tire,&sideforce,&traction);
    if(tire==(u8 *)model+1256 || tire==(u8 *)model+1348){
        traction*=D_80111130[FIELD(model,s8,12)*3+FIELD(model,s8,11)];
        traction*=1.0f+(D_80110F80[FIELD(model,s8,9)*6+FIELD(model,s16,1012)]-1.0f)*(1.0f-fabsf(FIELD(model,f32,1824)));
        traction*=FIELD(FIELD(model,void *,4),f32,24);
        sideforce*=FIELD(model,f32,1456)*(1.0f-FIELD(FIELD(model,void *,4),f32,28)*D_80123E18);
        if(poortract){
            if(D_80142DB0==2)sideforce*=D_80123E1C;
            else sideforce*=FIELD(model,f32,1460)*D_80123E20+D_80123E24;
        }
    }
    tireforcevec[2]=traction;tireforcevec[0]=sideforce;
    func_8009E820(tireforcevec,(f32 *)forcevec,tireuv);
    FIELD(tire,f32,80)=sideforce;FIELD(tire,f32,84)=traction;
}
