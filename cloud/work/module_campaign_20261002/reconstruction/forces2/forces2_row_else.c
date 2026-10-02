/* flags: -g0 -O2 -mips2 -G 0 -non_shared -Wab,-r4300_mul */
/* Direct ancestor: rushtherock/game/drivsym.c:forces2 and vecmath.h.
 * Native Y is vertical; the N64 port blends rear thrust selectively,
 * computes gravity each step, and measures the summed body contact force. */
typedef float f32;
typedef signed char s8;
typedef unsigned char u8;
typedef signed short s16;
typedef int s32;
typedef f32 Vec3[3];
typedef struct Parameters {f32 rear_force_blend;} Parameters;
typedef struct Model2056 {
    void *car;
    Parameters *parameters;
    u8 car_type;
    u8 to_force[7];
    Vec3 force,moment,acceleration,angular_acceleration,velocity,angular_velocity,drag;
    Vec3 tire_force[4];
    u8 to_body_force[48];
    Vec3 body_force[4];
    u8 to_center_force[48];
    Vec3 center_force;
    u8 to_gravity[216];
    Vec3 gravity;
    u8 to_position[28];
    Vec3 position;
    Vec3 previous_position;
    u8 to_world_gravity[140];
    Vec3 world_gravity;
    u8 to_basis[12];
    f32 basis[9];
    u8 to_gear[232];
    s16 gear;
    u8 to_no_thrust[431];
    s8 no_thrust;
    u8 to_mass[22];
    f32 mass;
    u8 to_road[80];
    s32 road[4];
    u8 to_contact_magnitude[44];
    f32 contact_magnitude;
    u8 to_player[370];
    s16 player;
    u8 to_mode[4];
    s8 mode;
    u8 tail[59];
} Model2056;
extern f32 D_80142764;
extern f32 D_80114178[];
extern s8 D_8011128C[][13];
extern void func_800A61B0(f32 *,f32 *,f32 *);
extern f32 func_8008B3C8(f32 *);
#define vecadd(a,b,r) {r[0]=a[0]+b[0];r[1]=a[1]+b[1];r[2]=a[2]+b[2];}
void func_800E1C30(Model2056 *m)
{
    f32 temp,body_force[3],blend;
    s32 row;
    if(m->no_thrust) {
        temp=(m->tire_force[2][2]+m->tire_force[3][2])*(f32).5;
        if(temp>0) {
            if(m->road[1]==8) m->tire_force[2][2]=0;
            else {
                blend=m->parameters->rear_force_blend;
                m->tire_force[2][2]=m->tire_force[2][2]*blend+temp*((f32)1.0-blend);
            }
            if(m->road[0]==8) m->tire_force[3][2]=0;
            else {
                blend=m->parameters->rear_force_blend;
                m->tire_force[3][2]=m->tire_force[3][2]*blend+temp*((f32)1.0-blend);
            }
        }
    }
    vecadd(m->tire_force[0],m->tire_force[1],m->force);
    vecadd(m->tire_force[2],m->force,m->force);
    vecadd(m->tire_force[3],m->force,m->force);
    m->world_gravity[1]=m->mass*D_80142764*(f32)-1.0;
    if(m->gear && m->position[0]<=m->previous_position[0]) {
        if(m->mode==2) row=m->player+1;
        else row=0;
        m->world_gravity[1]*=D_80114178[D_8011128C[row][m->car_type]];
    }
    func_800A61B0(m->world_gravity,m->gravity,m->basis);
    vecadd(m->gravity,m->force,m->force);
    vecadd(m->drag,m->force,m->force);
    vecadd(m->body_force[0],m->body_force[1],body_force);
    vecadd(body_force,m->body_force[2],body_force);
    vecadd(body_force,m->body_force[3],body_force);
    vecadd(m->force,body_force,m->force);
    m->contact_magnitude=func_8008B3C8(body_force);
    vecadd(m->center_force,m->force,m->force);
}
