/* flags: -g0 -O3 -mips2 -G 0 -non_shared -Wab,-r4300_mul */
typedef float f32;
typedef f32 Vec3[3];
typedef struct Basis {Vec3 first,second,third;} Basis;
extern void func_800A61B0(const Vec3 *,Vec3 *,const Basis *);
extern f32 cosf(f32),sinf(f32);
void camera_collision_avoid(const Vec3 *origin,const Vec3 *first,const Vec3 *second,f32 angle,const Basis *transform,const Basis *source,Basis *output,Vec3 *position) {
 Vec3 point;
 point[0]=(*second)[1]*(*first)[2]-(*first)[1]*(*second)[2];
 point[1]=(*second)[2]*(*first)[0]-(*first)[2]*(*second)[0];
 point[2]=(*second)[0]*(*first)[1]-(*first)[0]*(*second)[1];
 point[0]=(*origin)[0]+point[0];
 point[1]=(*origin)[1]+point[1];
 point[2]=(*origin)[2]+point[2];
 func_800A61B0(&source->second,&output->second,transform);
 output->first[0]=cosf(angle);
 output->first[1]=0.0f;
 output->first[2]=-sinf(angle);
 output->third[0]=output->first[1]*output->second[2]-output->second[1]*output->first[2];
 output->third[1]=output->first[2]*output->second[0]-output->second[2]*output->first[0];
 output->third[2]=output->first[0]*output->second[1]-output->second[0]*output->first[1];
 output->first[0]=output->second[1]*output->third[2]-output->third[1]*output->second[2];
 output->first[1]=output->second[2]*output->third[0]-output->third[2]*output->second[0];
 output->first[2]=output->second[0]*output->third[1]-output->third[0]*output->second[1];
 func_800A61B0(&point,position,output);
}

void func_800A61B0(const Vec3 *in,Vec3 *out,const Basis *basis) {
 const f32 *a=*in;f32 *b=*out;const f32 *m=(const f32 *)basis;
 b[0]=(a[0]*m[0]+a[1]*m[1])+a[2]*m[2];
 b[1]=(a[0]*m[3]+a[1]*m[4])+a[2]*m[5];
 b[2]=(a[0]*m[6]+a[1]*m[7])+a[2]*m[8];
}
