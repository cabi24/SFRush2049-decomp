/* flags: -g0 -O3 -mips2 -G 0 -non_shared -Wab,-r4300_mul */
typedef int s32;
typedef struct Quaternion {float c[4];} Quaternion;
extern float D_80123E78,D_80123E7C,D_80123E80;
extern float sinf(float);
extern float func_8009C3F8(s32,float);
void func_800BFD8C(float fraction,Quaternion *first,Quaternion *second,Quaternion *out) {
    float cosine,angle,inverse,left,right,difference;
    s32 i;
    if(fraction<D_80123E78) {
        out->c[0]=first->c[0];out->c[1]=first->c[1];out->c[2]=first->c[2];out->c[3]=first->c[3];
        return;
    }
    if(D_80123E7C<fraction) {
        out->c[0]=second->c[0];out->c[1]=second->c[1];out->c[2]=second->c[2];out->c[3]=second->c[3];
        return;
    }
    cosine=second->c[3]*first->c[3]+((first->c[0]*second->c[0]+first->c[1]*second->c[1])+first->c[2]*second->c[2]);
    cosine*=D_80123E7C;
    if(cosine<0.0f) {
        cosine=-cosine;
        out->c[0]=-second->c[0];out->c[1]=-second->c[1];out->c[2]=-second->c[2];out->c[3]=-second->c[3];
    } else {
        out->c[0]=second->c[0];out->c[1]=second->c[1];out->c[2]=second->c[2];out->c[3]=second->c[3];
    }
    if(cosine<D_80123E80) {
        angle=func_8009C3F8(1,cosine);
        inverse=1.0f/sinf(angle);
        left=sinf((1.0f-fraction)*angle)*inverse;
        right=sinf(fraction*angle)*inverse;
        out->c[0]=out->c[0]*right+first->c[0]*left;
        out->c[1]=out->c[1]*right+first->c[1]*left;
        out->c[2]=out->c[2]*right+first->c[2]*left;
        out->c[3]=out->c[3]*right+first->c[3]*left;
    } else {
        for(i=0;i<4;i++) {
            difference=out->c[i]-first->c[i];
            if(difference>1.0f) difference-=2.0f;
            if(difference< -1.0f) difference+=2.0f;
            out->c[i]=first->c[i]+fraction*difference;
            if(out->c[i]>1.0f) out->c[i]-=2.0f;
            else if(out->c[i]< -1.0f) out->c[i]+=2.0f;
        }
    }
}
