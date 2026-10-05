/* flags: -g0 -O2 -mips2 -G 0 -non_shared */
typedef float f32;
typedef struct Matrix3 {f32 xuv[3],yuv[3],zuv[3];} Matrix3;
#define CopyVector(v1,v2) (v2[0]=v1[0],v2[1]=v1[1],v2[2]=v1[2])
#define CrossVector(v1,v2,r) (r[0]=v1[1]*v2[2]-v1[2]*v2[1], \
                            r[1]=v1[2]*v2[0]-v1[0]*v2[2], \
                            r[2]=v1[0]*v2[1]-v1[1]*v2[0])
extern f32 D_801141C8[3],D_8012388C;
void vector_copy_scale(f32 *,f32 *);
f32 func_8008B3C8(f32 *);
void vector_normalize_length(f32 lookdir[3],Matrix3 *mat)
{
    f32 length,reciprocal;
    CopyVector(lookdir,mat->zuv);
    vector_copy_scale(mat->zuv,mat->zuv);
    mat->xuv[0]=lookdir[2];
    mat->xuv[1]=0.0f;
    mat->xuv[2]=-lookdir[0];
    length=func_8008B3C8(mat->xuv);
    if(length<=D_8012388C) {
        CopyVector(D_801141C8,mat->xuv);
    } else {
        reciprocal=1.0f/length;
        mat->xuv[0]*=reciprocal;
        mat->xuv[1]*=reciprocal;
        mat->xuv[2]*=reciprocal;
    }
    CrossVector(mat->zuv,mat->xuv,mat->yuv);
    CrossVector(mat->yuv,mat->zuv,mat->xuv);
}
