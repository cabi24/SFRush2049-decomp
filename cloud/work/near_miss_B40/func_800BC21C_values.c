/* flags: -g0 -O2 -mips2 -G 0 -non_shared -Wab,-r4300_mul */
typedef float f32;
extern f32 D_80123E04,D_80123E08;
f32 func_800BC21C(f32 *vector) {
    f32 value;
    f32 result;
    f32 z;
    value=vector[0];
    z=vector[2];
    result=value;
    if(z>0.0f) {
        if(z<400.0f) result=value/z;
        else result=value*D_80123E04;
    } else if(z<0.0f) {
        if(z>-400.0f) result=(-value)/z;
        else result=value*D_80123E08;
    }
    return result;
}
