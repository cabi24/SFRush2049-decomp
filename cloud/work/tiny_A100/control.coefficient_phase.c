/* flags: -g0 -O2 -mips2 -G 0 -non_shared */
typedef float f32;
void func_800BFBE8(f32 *matrix,f32 *quaternion,int normalized)
{
    f32 x,y,z,xx,yy,zz,first,second;
    {
    f32 scale;
    if(normalized==0) {
        f32 norm=((quaternion[0]*quaternion[0]+quaternion[1]*quaternion[1])+quaternion[2]*quaternion[2])+quaternion[3]*quaternion[3];
        if(norm>0.0f)scale=2.0f/norm;
        else scale=0.0f;
    } else scale=2.0f;
    x=quaternion[0]*scale;
    y=quaternion[1]*scale;
    z=quaternion[2]*scale;
    }
    xx=quaternion[0]*x;
    yy=quaternion[1]*y;
    zz=quaternion[2]*z;
    matrix[0]=1.0f-(yy+zz);
    matrix[4]=1.0f-(xx+zz);
    matrix[8]=1.0f-(xx+yy);
    first=quaternion[0]*y;
    second=quaternion[3]*z;
    matrix[1]=-(first+second);
    matrix[3]=-(first-second);
    first=quaternion[1]*z;
    second=quaternion[3]*x;
    matrix[5]=-(first+second);
    matrix[7]=-(first-second);
    first=quaternion[0]*z;
    second=quaternion[3]*y;
    matrix[2]=first-second;
    matrix[6]=first+second;
}
