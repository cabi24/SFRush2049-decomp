/* flags: -g0 -O2 -mips2 -G 0 -non_shared -Wab,-r4300_mul */
typedef float f32;
void menu_vibration_test(const f32 *q,f32 *matrix) {
    f32 x,y,z,w;
    f32 squares[4],sumxy,sum,scale,diff,diagonal;
    f32 yz,xw,yw,xz,zw,xy;
    x=q[0]; y=q[1]; z=q[2]; w=q[3];
    squares[0]=x*x;
    squares[1]=y*y;
    squares[2]=z*z;
    squares[3]=w*w;
    sumxy=squares[0]+squares[1];
    sum=(sumxy+squares[2])+squares[3];
    diagonal=(sumxy-squares[2])-squares[3];
    if(sum>0.0f) scale=1.0f/sum;
    else scale=0.0f;
    matrix[0]=diagonal*scale;
    yz=y*z;
    xw=x*w;
    matrix[1]=((yz-xw)*2.0f)*scale;
    yw=y*w;
    xz=x*z;
    matrix[2]=((yw+xz)*2.0f)*scale;
    matrix[3]=((yz+xw)*2.0f)*scale;
    diff=squares[0]-squares[1];
    matrix[4]=((diff+squares[2])-squares[3])*scale;
    zw=z*w;
    xy=x*y;
    matrix[5]=((zw-xy)*2.0f)*scale;
    matrix[6]=((yw-xz)*2.0f)*scale;
    matrix[7]=((zw+xy)*2.0f)*scale;
    matrix[8]=((diff-squares[2])+squares[3])*scale;
}
