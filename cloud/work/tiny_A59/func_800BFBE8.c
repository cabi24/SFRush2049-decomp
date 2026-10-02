/* flags: -g0 -O2 -mips2 -G 0 -non_shared */
typedef float f32;typedef int s32;
void func_800BFBE8(f32 *out,f32 *q,s32 normalized) {
 f32 x,y,z,w,n,scale,scaled[3],square[3],first,second;
 if(!normalized) {
  x=q[0];y=q[1];z=q[2];w=q[3];n=w*w+((x*x+y*y)+z*z);
  if(n>0.0f)scale=2.0f/n;else scale=0.0f;
 }else {scale=2.0f;x=q[0];y=q[1];z=q[2];}
 scaled[0]=x*scale;scaled[1]=y*scale;scaled[2]=z*scale;
 square[0]=x*scaled[0];square[1]=y*scaled[1];square[2]=z*scaled[2];
 out[0]=1.0f-(square[1]+square[2]);out[4]=1.0f-(square[0]+square[2]);out[8]=1.0f-(square[0]+square[1]);
 first=q[0]*scaled[1];second=q[3]*scaled[2];out[1]=-(first+second);out[3]=-(first-second);
 first=q[1]*scaled[2];second=q[3]*scaled[0];out[5]=-(first+second);out[7]=-(first-second);
 first=q[0]*scaled[2];second=q[3]*scaled[1];out[2]=first-second;out[6]=first+second;
}
