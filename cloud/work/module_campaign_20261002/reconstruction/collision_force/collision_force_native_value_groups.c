/* flags: -g0 -O3 -mips2 -G 0 -non_shared -Wab,-r4300_mul */
/* Direct ancestor: collision.c:setFBCollisionForce; N64 adds item effects
 * and applies the mass-weighted opposite collision force to both cars. */
typedef float f32;
typedef signed char s8;
typedef unsigned char u8;
typedef signed short s16;
typedef f32 Vec3[3];
typedef struct Model2056 {
    u8 to_body_corner[244];
    Vec3 body_corner[4];
    Vec3 center_force;
    u8 to_world_velocity[240];
    Vec3 world_velocity;
    u8 to_speed[452];
    f32 speed;
    u8 to_inverse_mass[464];
    f32 inverse_mass;
    u8 to_threshold[116];
    f32 collision_threshold;
    s8 controlled;
    u8 to_hit_target[131];
    s16 hit_target;
    u8 to_reckon_velocity[194];
    Vec3 reckon_velocity,reckon_position;
    f32 reckon_basis[9];
    u8 to_player[2];
    s16 player;
    u8 to_mode[4];
    s8 mode;
    u8 tail[59];
} Model2056;
typedef struct Car952 {
    u8 to_blocked[856];
    s8 blocked,control,unknown,team;
    u8 to_slow[3];
    s8 slow;
    u8 to_item[36];
    s8 item,item_count;
    u8 to_cooldown[38];
    f32 cooldown;
    u8 tail[8];
} Car952;
extern Car952 player_array[];
extern int gameplay_mode;
extern s8 D_8012E67C[],D_8017A634;
extern f32 D_80124108,D_8012410C,D_80124110,D_80124114;
extern void menu_load_options(Model2056 *,Model2056 *,Model2056 *,f32 *);
extern void func_8038d3a4(Car952 *,Car952 *,int);
extern void func_800A61B0(f32 *,f32 *,f32 *);
extern void func_8009E820(f32 *,f32 *,f32 *);
extern f32 func_8008B3C8(f32 *);
extern f32 fabsf(f32);
#pragma intrinsic(fabsf)
#define vecsub(a,b,r) {r[0]=a[0]-b[0];r[1]=a[1]-b[1];r[2]=a[2]-b[2];}
#define vecadd(a,b,r) {r[0]=a[0]+b[0];r[1]=a[1]+b[1];r[2]=a[2]+b[2];}
#define scalmul(a,b,r) {r[0]=a[0]*(b);r[1]=a[1]*(b);r[2]=a[2]*(b);}
void menu_options_screen(Model2056 *m,Model2056 *m1,Model2056 *m2,f32 *dir,f32 *pos)
{
    f32 force[3],temp[3];
    f32 ratio1,ratio2,magnitude;
    f32 rvel[3],cent[3],other_cent[3],inverse_mass;
    f32 *basis1,*basis2;
    int beside,behind,local_car;
    Car952 *car1,*car2;
    car1=&player_array[m1->player];
    car2=&player_array[m2->player];
    if(m1->player>=0 && m2->player>=0 && gameplay_mode!=6) {
        if(car1->slow) {
            m2->controlled=1;
            car1->slow=0;
            scalmul(m2->world_velocity,(f32).5,m2->world_velocity);
        }
        if(car2->slow) {
            m1->controlled=1;
            car2->slow=0;
            scalmul(m1->world_velocity,(f32).5,m1->world_velocity);
        }
    }
    basis1=m1->reckon_basis;
    basis2=m2->reckon_basis;
    vecsub(m1->reckon_position,m2->reckon_position,temp);
    func_800A61B0(temp,cent,basis2);
    beside=(cent[2]<m2->body_corner[0][2] && cent[2]>m2->body_corner[3][2]);
    behind=(cent[0]<m2->body_corner[0][0] && cent[0]>m2->body_corner[3][0]);
    if(gameplay_mode==6 && D_8012E67C[car1->team]!=D_8012E67C[car2->team]) {
        if(car1->item==5 && m2->hit_target==-1 && car1->cooldown<=0 && !car2->blocked) {
            vecsub(m2->reckon_position,m1->reckon_position,temp);
            func_800A61B0(temp,other_cent,basis1);
            if(other_cent[2]>(f32)2.25) {
                car1->item_count--;
                car1->cooldown=D_80124108;
                func_8038d3a4(car1,car2,(int)((f32)160.0+m1->speed*(f32)7.0));
            }
        } else if(car2->item==5 && m1->hit_target==-1 && car2->cooldown<=0 && !car1->blocked) {
            if(cent[2]>(f32)2.25) {
                car2->item_count--;
                car2->cooldown=D_8012410C;
                func_8038d3a4(car2,car1,(int)((f32)160.0+m2->speed*(f32)7.0));
            }
        }
    }
    if(beside && behind) {
        menu_load_options(m,m1,m2,dir);
        return;
    }
    vecsub(m1->reckon_velocity,m2->reckon_velocity,temp);
    func_800A61B0(temp,rvel,basis2);
    if(beside) force[2]=0;
    else {
        force[2]=rvel[2]*(f32)2000.0;
        if(pos[2]>0 && force[2]>(f32)-4000.0) force[2]=(f32)-4000.0;
        else if(pos[2]<0 && force[2]<(f32)4000.0) force[2]=(f32)4000.0;
    }
    if(behind) force[0]=0;
    else {
        force[0]=rvel[0]*(f32)2000.0;
        if(pos[0]>0) {
            if(force[0]>(f32)-4000.0) force[0]=(f32)-4000.0;
            if(beside) {
                temp[0]=pos[0]-m2->body_corner[2][0];
                temp[0]*=D_80124110*fabsf(temp[0]);
                if(force[0]>temp[0]) force[0]=temp[0];
            }
        } else if(pos[0]<0) {
            if(force[0]<(f32)4000.0) force[0]=(f32)4000.0;
            if(beside) {
                temp[0]=pos[0]-m2->body_corner[1][0];
                temp[0]*=D_80124114*fabsf(temp[0]);
                if(force[0]<temp[0]) force[0]=temp[0];
            }
        }
    }
    force[1]=rvel[1]*(f32)100.0;
    inverse_mass=(m1->inverse_mass+m2->inverse_mass)*(f32).5;
    func_8009E820(force,temp,basis2);
    func_800A61B0(temp,cent,basis1);
    ratio1=m2->inverse_mass/inverse_mass;
    scalmul(cent,ratio1,cent);
    vecsub(m1->center_force,cent,m1->center_force);
    local_car=(m1->mode==2 || m2->mode==2);
    if(D_8017A634==1 || (D_8017A634==2 && local_car) ||
       (magnitude=func_8008B3C8(cent))>m1->collision_threshold*(f32).5) {
        if(gameplay_mode!=6 && !m1->controlled && car1->control!=2) m1->controlled=1;
    }
    ratio2=m1->inverse_mass/inverse_mass;
    scalmul(force,ratio2,force);
    vecadd(force,m2->center_force,m2->center_force);
    if(D_8017A634==1 || (D_8017A634==2 && local_car) ||
       (magnitude=func_8008B3C8(force))>m2->collision_threshold*(f32).5) {
        if(gameplay_mode!=6 && !m2->controlled && car2->control!=2) m2->controlled=1;
    }
}
