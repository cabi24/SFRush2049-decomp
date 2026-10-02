/* flags: -g0 -O3 -mips2 -G 0 -non_shared -Wab,-r4300_mul */
typedef int s32;
extern float D_80123E78,D_80123E7C,D_80123E80;
extern float sinf(float);
extern float func_8009C3F8(s32,float);
void func_800BFD8C(float fraction,float *first,float *second,float *out) {
    float cosine,difference;
    s32 i;
    if(fraction<D_80123E78) {
        out[0]=first[0];out[1]=first[1];out[2]=first[2];out[3]=first[3];
        return;
    }
    if(D_80123E7C<fraction) {
        out[0]=second[0];out[1]=second[1];out[2]=second[2];out[3]=second[3];
        return;
    }
    cosine=second[3]*first[3]+((first[0]*second[0]+first[1]*second[1])+first[2]*second[2]);
    cosine*=D_80123E7C;
    if(cosine<0.0f) {
        cosine=-cosine;
        out[0]=-second[0];out[1]=-second[1];out[2]=-second[2];out[3]=-second[3];
    } else {
        out[0]=second[0];out[1]=second[1];out[2]=second[2];out[3]=second[3];
    }
    if(cosine<D_80123E80) {
        float angle,inverse,weights[2];
        angle=func_8009C3F8(1,cosine);
        inverse=1.0f/sinf(angle);
        weights[0]=sinf((1.0f-fraction)*angle)*inverse;
        weights[1]=sinf(fraction*angle)*inverse;
        out[0]=out[0]*weights[1]+first[0]*weights[0];
        out[1]=out[1]*weights[1]+first[1]*weights[0];
        out[2]=out[2]*weights[1]+first[2]*weights[0];
        out[3]=out[3]*weights[1]+first[3]*weights[0];
    } else {
        for(i=0;i<4;i++) {
            difference=out[i]-first[i];
            if(difference>1.0f) difference-=2.0f;
            if(difference< -1.0f) difference+=2.0f;
            out[i]=first[i]+fraction*difference;
            if(out[i]>1.0f) out[i]-=2.0f;
            else if(out[i]< -1.0f) out[i]+=2.0f;
        }
    }
}
