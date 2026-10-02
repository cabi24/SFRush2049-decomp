/* flags: -g0 -O1 -mips2 -G 0 -non_shared -Wab,-r4300_mul */
typedef float f32;
void menu_vibration_test(const f32 *q,f32 *matrix) {
    f32 x,y,z,w;
    f32 x2,y2,z2,w2,sumxy,sum,scale,diff;
    f32 yz,xw,yw,xz,zw,xy;
    x=q[0]; y=q[1]; z=q[2]; w=q[3];
    x2=x*x;
    y2=y*y;
    z2=z*z;
    w2=w*w;
    sumxy=x2+y2;
    sum=(sumxy+z2)+w2;
    if(sum>0.0f) scale=1.0f/sum;
    else scale=0.0f;
    matrix[0]=((sumxy-z2)-w2)*scale;
    yz=y*z;
    xw=x*w;
    matrix[1]=((yz-xw)*2.0f)*scale;
    yw=y*w;
    xz=x*z;
    matrix[2]=((yw+xz)*2.0f)*scale;
    matrix[3]=((yz+xw)*2.0f)*scale;
    diff=x2-y2;
    matrix[4]=((diff+z2)-w2)*scale;
    zw=z*w;
    xy=x*y;
    matrix[5]=((zw-xy)*2.0f)*scale;
    matrix[6]=((yw-xz)*2.0f)*scale;
    matrix[7]=((zw+xy)*2.0f)*scale;
    matrix[8]=((diff-z2)+w2)*scale;
}
