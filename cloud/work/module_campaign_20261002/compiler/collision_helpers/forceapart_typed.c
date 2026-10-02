/* Native ForceApart at CE1EC, adapted from collision.c:ForceApart.
 * Four actual pointer inputs; original unused pos argument removed on N64.
 * N64 applies equal opposite center forces through both reckon bases. */
typedef float f32;typedef unsigned char u8;
typedef struct Vec3 {f32 x,y,z;} Vec3;
typedef struct Model2056 {u8 to_force[292];Vec3 center_force;u8 to_reckon_basis[1648];f32 reckon_basis[9];u8 tail[68];} Model2056;
extern f32 D_801240FC,D_80124100,D_80124104;
extern f32 sqrtf(f32);
#pragma intrinsic(sqrtf)
extern void func_800A61B0(f32 *,f32 *,f32 *);
#define vecadd(a,b,r) {r.x=a.x+b.x;r.y=a.y+b.y;r.z=a.z+b.z;}
#define vecsub(a,b,r) {r.x=a.x-b.x;r.y=a.y-b.y;r.z=a.z-b.z;}
void menu_load_options(Model2056 *m,Model2056 *m1,Model2056 *m2,Vec3 *dir) {
 Vec3 transformed,force;
 f32 invdist;
 Model2056 *mother;
 f32 *fp,*dp;
 mother=m==m1 ? m2 : m1;
 invdist=dir->z*dir->z+(dir->x*dir->x+dir->y*dir->y);
 if(invdist<D_801240FC) {
  force.y=force.z=0.0f;
  force.x=D_80124100;
 } else {
  invdist=D_80124104/sqrtf(invdist);
  for(fp=(f32 *)&force,dp=(f32 *)dir;fp<(f32 *)(&force+1);fp++,dp++)*fp=*dp*invdist;
 }
 func_800A61B0((f32 *)&force,(f32 *)&transformed,m->reckon_basis);
 vecadd(transformed,m->center_force,m->center_force);
 func_800A61B0((f32 *)&force,(f32 *)&transformed,mother->reckon_basis);
 vecsub(mother->center_force,transformed,mother->center_force);
}
