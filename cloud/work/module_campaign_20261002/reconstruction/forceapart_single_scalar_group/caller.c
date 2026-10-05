/* Native ForceApart at CE1EC, adapted from collision.c:ForceApart.
 * Four actual pointer inputs; original unused pos argument removed on N64.
 * N64 applies equal opposite center forces through both reckon bases. */
typedef float f32;typedef unsigned char u8;
typedef f32 Vec3[3];
typedef struct Model2056 {u8 to_force[292];Vec3 center_force;u8 to_reckon_basis[1648];f32 reckon_basis[9];u8 tail[68];} Model2056;
extern f32 D_801240FC,D_80124100,D_80124104;
extern f32 sqrtf(f32);
#pragma intrinsic(sqrtf)
extern void func_800A61B0(f32 *,f32 *,f32 *);
#define vecadd(a,b,r) {r[0]=a[0]+b[0];r[1]=a[1]+b[1];r[2]=a[2]+b[2];}
#define vecsub(a,b,r) {r[0]=a[0]-b[0];r[1]=a[1]-b[1];r[2]=a[2]-b[2];}
void menu_load_options(Model2056 *m,Model2056 *m1,Model2056 *m2,f32 dir[3]) {
 Model2056 *mother;
 f32 invdist;
 Vec3 force,transformed;
 int i;
 mother=m==m1 ? m2 : m1;
 #define dotprod(a,b) ((a[0]*b[0]+a[1]*b[1])+a[2]*b[2])
 invdist=dotprod(dir,dir);
 if(invdist<D_801240FC) {
  force[1]=force[2]=0.0f;
  force[0]=D_80124100;
 } else {
  invdist=D_80124104/sqrtf(invdist);
  for(i=0;i<3;i++)force[i]=dir[i]*invdist;
 }
 func_800A61B0((f32 *)&force,(f32 *)&transformed,m->reckon_basis);
 vecadd(m->center_force,transformed,m->center_force);
 func_800A61B0((f32 *)&force,(f32 *)&transformed,mother->reckon_basis);
 vecsub(mother->center_force,transformed,mother->center_force);
}
