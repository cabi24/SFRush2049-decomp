/* flags: -g0 -O3 -mips2 -G 0 -non_shared */
/*
 * camera_collision_avoid: offset `origin` by first x second (cross product) into
 * `point`; transform source->second by `transform` into output->second
 * (func_800A61B0(in, out, basis)); set output->first = (cos a, 0, -sin a) and
 * re-orthogonalise: third = first x second, first = second x third; finally
 * transform `point` by the new basis into *position.  -O3 only (-O2 allocates
 * differently).  Whole-program unit: EQUAL.
 * Body: cloud/work/near_miss_B57/camera_collision_avoid_native.c unchanged.
 * Shaping quirk: an unused 16-byte local array declared after `point`
 * (`f32 pad[4]`) supplies retail's 120-byte frame and puts point at sp+108;
 * without it the code is identical but the frame is 104 (14 rows).  A
 * Vec3-sized unused local does not move the frame.
 */
typedef float f32;
typedef struct Vec3 {f32 x,y,z;} Vec3;
typedef struct Basis {Vec3 first,second,third;} Basis;
extern void func_800A61B0(const Vec3 *,Vec3 *,const Basis *);
extern f32 cosf(f32),sinf(f32);
void camera_collision_avoid(const Vec3 *origin,const Vec3 *first,const Vec3 *second,f32 angle,const Basis *transform,const Basis *source,Basis *output,Vec3 *position) {
 Vec3 point;
 f32 pad[4];
 point.x=second->y*first->z-first->y*second->z;
 point.y=second->z*first->x-first->z*second->x;
 point.z=second->x*first->y-first->x*second->y;
 point.x=origin->x+point.x;
 point.y=origin->y+point.y;
 point.z=origin->z+point.z;
 func_800A61B0(&source->second,&output->second,transform);
 output->first.x=cosf(angle);
 output->first.y=0.0f;
 output->first.z=-sinf(angle);
 output->third.x=output->first.y*output->second.z-output->second.y*output->first.z;
 output->third.y=output->first.z*output->second.x-output->second.z*output->first.x;
 output->third.z=output->first.x*output->second.y-output->second.x*output->first.y;
 output->first.x=output->second.y*output->third.z-output->third.y*output->second.z;
 output->first.y=output->second.z*output->third.x-output->third.z*output->second.x;
 output->first.z=output->second.x*output->third.y-output->third.x*output->second.y;
 func_800A61B0(&point,position,output);
}
