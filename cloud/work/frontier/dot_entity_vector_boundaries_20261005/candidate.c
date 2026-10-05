/* NONMATCH: authentic macro boundary control; see README.md. */
/* flags: -g0 -O3 -mips2 -G 0 -non_shared -Wab,-r4300_mul */
typedef float f32;
typedef struct Basis {f32 m[9];} Basis;
extern void *input_deadzone_apply(f32 *,f32 *,Basis *,f32,int,int);
extern f32 func_8008E0B8(f32 *);
#define SubVector(v1,v2,r)		(r[0] = v1[0]-v2[0], r[1] = v1[1]-v2[1], r[2] = v1[2]-v2[2])
#define ScaleAddVector(a,s,b,r)	(r[0] = (a[0]*(s)+b[0]), r[1] = (a[1]*(s)+b[1]), r[2] = (a[2]*(s)+b[2]))

int entity_iterate(f32 *first,f32 *second) {
 Basis basis;
 f32 difference[3];
 if(input_deadzone_apply(second,first,&basis,0.5f,1,6)) {
  SubVector(second,first,difference);
  func_8008E0B8(difference);
  ScaleAddVector(difference,0.25f,first,first);
 }
 return 0;
}
