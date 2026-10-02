/* flags: -g0 -O2 -mips2 -G 0 -non_shared -Wab,-r4300_mul */
/* Direct ancestor: rushtherock/game/drivsym.c:positions and vecmath.h.
 * N64 integrates ordinary world position, normalizes periodically, and
 * caps tire penetration before updating world-space contact corners. */
typedef float f32;
typedef signed char s8;
typedef unsigned char u8;
typedef unsigned int u32;
typedef f32 Vec3[3];
typedef struct Car {
    u8 to_tire_corner[112];
    Vec3 tire_corner[4];
} Car;
typedef struct Model2056 {
    Car *car;
    u8 to_acceleration[36];
    Vec3 acceleration,angular_acceleration,velocity,angular_velocity;
    u8 to_body_corner[156];
    Vec3 body_corner[4];
    u8 to_world_acceleration[240];
    Vec3 world_acceleration,world_velocity,position,previous_position;
    Vec3 world_tire_corner[4],world_body_corner[4];
    u8 to_orientation[60];
    Vec3 orientation;
    Vec3 basis[3];
    u8 to_penetration[716];
    f32 penetration[4];
    u8 to_dt[72];
    f32 dt;
    u8 to_flags[216];
    u32 flags;
    u8 tail[244];
} Model2056;
extern int gameplay_mode;
extern s8 D_801427A1;
extern void func_8009E820(f32 *,f32 *,f32 *);
extern void func_800A61B0(f32 *,f32 *,f32 *);
extern void sound_position_set(f32 *,f32 *);
extern void menu_video_settings(Model2056 *);
extern f32 func_8008B424(f32 *);
#define veccopy(a,r) {r[0]=a[0];r[1]=a[1];r[2]=a[2];}
#define vecadd(a,b,r) {r[0]=a[0]+b[0];r[1]=a[1]+b[1];r[2]=a[2]+b[2];}
#define scalmul(a,b,r) {r[0]=a[0]*(b);r[1]=a[1]*(b);r[2]=a[2]*(b);}
void func_800D0424(Model2056 *m)
{
    f32 temp[3],transformed[3],factor,penetration;
    f32 *bp,*rwp;
    int i;
    veccopy(m->position,m->previous_position);
    func_8009E820(m->acceleration,m->world_acceleration,m->basis[0]);
    func_8009E820(m->velocity,m->world_velocity,m->basis[0]);
    scalmul(m->world_velocity,m->dt,temp);
    vecadd(m->position,temp,m->position);
    scalmul(m->angular_velocity,m->dt,temp);
    if(gameplay_mode==4) {
        vecadd(m->orientation,temp,m->orientation);
    }
    sound_position_set(temp,m->basis[0]);
    if((m->flags&0x3ff)==0x3ff) {
        if(D_801427A1) menu_video_settings(m);
        else {
            factor=func_8008B424(m->basis[0]);
            scalmul(m->basis[0],factor,m->basis[0]);
            factor=func_8008B424(m->basis[1]);
            scalmul(m->basis[1],factor,m->basis[1]);
            factor=func_8008B424(m->basis[2]);
            scalmul(m->basis[2],factor,m->basis[2]);
        }
    }
    func_800A61B0(m->world_velocity,m->velocity,m->basis[0]);
    temp[1]=0;
    for(i=0;i<4;i++) {
        penetration=m->penetration[i]-(f32).5;
        if(temp[1]<penetration) {
            temp[1]=penetration;
            m->penetration[i]=(f32).5;
        }
    }
    if(temp[1]>0) {
        temp[0]=0;
        temp[2]=0;
        func_8009E820(temp,transformed,m->basis[0]);
        vecadd(m->position,transformed,m->position);
    }
    for(i=0,rwp=m->world_tire_corner[0];i<4;i++,rwp+=3) {
        temp[0]=m->car->tire_corner[i][0];
        temp[1]=m->penetration[i]+m->car->tire_corner[i][1];
        temp[2]=m->car->tire_corner[i][2];
        func_8009E820(temp,rwp,m->basis[0]);
        vecadd(rwp,m->position,rwp);
    }
    for(i=0,bp=m->body_corner[0],rwp=m->world_body_corner[0];i<4;i++,bp+=3,rwp+=3) {
        func_8009E820(bp,rwp,m->basis[0]);
        vecadd(rwp,m->position,rwp);
    }
}
