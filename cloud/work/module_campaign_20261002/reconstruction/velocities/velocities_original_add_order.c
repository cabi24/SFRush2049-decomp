/* flags: -g0 -O2 -mips2 -G 0 -non_shared -Wab,-r4300_mul */
/* Direct ancestor: rushtherock/game/drivsym.c:velocities.
 * Native extends this with angular damping and near-rest clamps. */
typedef float f32;
typedef signed short s16;
typedef signed char s8;
typedef unsigned char u8;
typedef f32 Vec3[3];
typedef struct Model2056 {
    u8 prefix[40]; Vec3 acceleration,angular_acceleration,velocity,angular_velocity;
    u8 to_basis[660]; f32 basis[9];
    u8 to_throttle[192]; f32 throttle,brake;
    u8 to_speed[24]; f32 speed;
    u8 to_suspension[504]; f32 suspension[4];
    u8 to_dt[56]; f32 dt;
    u8 to_controlled[8]; s8 controlled;
    u8 to_angular_damping[15]; f32 angular_damping;
    u8 to_player[370]; s16 player; u8 tail[64];
} Model2056;
typedef struct Car952 {u8 prefix[857]; s8 control; u8 tail[94];} Car952;
extern Car952 player_array[];
extern f32 D_801243B4,D_801243B8,D_801243BC;
extern f32 D_80114184,D_80114188;
extern f32 func_8008B3C8(f32 *);
extern f32 fabsf(f32);
#pragma intrinsic(fabsf)
#define scalmul(a,b,r) {r[0]=a[0]*(b);r[1]=a[1]*(b);r[2]=a[2]*(b);}
#define vecadd(a,b,r) {r[0]=a[0]+b[0];r[1]=a[1]+b[1];r[2]=a[2]+b[2];}
void func_800E114C(Model2056 *m)
{
    f32 temp[3],velfact,angular_speed,angular_gain;
    scalmul(m->acceleration,m->dt,temp);
    vecadd(m->velocity,temp,m->velocity);
    if (m->throttle<D_801243B4) {
        if (fabsf(m->velocity[0])<D_801243B8) m->velocity[0]=0;
        if (fabsf(m->velocity[2])<D_801243B8) m->velocity[2]=0;
    }
    if (player_array[m->player].control!=2)
        m->speed=func_8008B3C8(m->velocity);
    if (m->speed>(f32)300.0) {
        velfact=(f32)300.0/m->speed;
        scalmul(m->velocity,velfact,m->velocity);
        return;
    }
    scalmul(m->angular_acceleration,m->dt,temp);
    if (m->angular_damping!=0) {
        temp[0]-=m->angular_velocity[0]*D_801243BC;
        temp[1]-=m->angular_velocity[1]*D_801243BC;
        temp[2]-=m->angular_velocity[2]*D_801243BC;
    }
    vecadd(m->angular_velocity,temp,m->angular_velocity);
    if ((m->suspension[0]>0 && m->suspension[1]>0 &&
         m->suspension[2]>0 && m->suspension[3]>0) || m->controlled) {
        angular_speed=func_8008B3C8(m->angular_velocity);
        if (m->controlled) angular_gain=D_80114188;
        else angular_gain=D_80114184;
        velfact=angular_speed*angular_speed*angular_gain;
        m->angular_velocity[0]-=m->angular_velocity[0]*velfact;
        m->angular_velocity[1]-=m->angular_velocity[1]*velfact;
        m->angular_velocity[2]-=m->angular_velocity[2]*velfact;
    }
    if (m->speed<(f32)3.0 && (m->brake>(f32).5 || m->basis[4]<0)) {
        m->velocity[0]=0;
        m->velocity[2]=0;
    }
    if (m->speed<(f32)3.0 && m->basis[4]<(f32)-.75 &&
        func_8008B3C8(m->angular_velocity)<(f32).5) {
        m->angular_velocity[0]=0;
        m->angular_velocity[1]=0;
        m->angular_velocity[2]=0;
    }
}
